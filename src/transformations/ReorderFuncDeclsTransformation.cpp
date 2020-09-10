#include "../../include/transformations/ReorderFuncDeclsTransformation.h"
#include <iostream>

using llvm::isa, llvm::cast;
using std::unique_ptr, std::sort, std::shuffle, std::find, std::vector, std::string;
using clang::SourceRange;

ReorderFuncDeclsVisitor::ReorderFuncDeclsVisitor(Rewriter * rewriter, mt19937 * gen, const bool test) :
                                                 rewriter(rewriter), gen(gen), test(test) {}

bool ReorderFuncDeclsVisitor::VisitFunctionDecl(FunctionDecl * decl) {
    if (decl->getName().str() == "main") {
        mainDecl = decl;
    }
    return true;
}

bool ReorderFuncDeclsVisitor::isFuncDeclProcessed(FunctionDecl * decl) {
    return find(funcdecls.begin(), funcdecls.end(), decl) != funcdecls.end();
}

bool ReorderFuncDeclsVisitor::VisitCallExpr(CallExpr * call) {
    FunctionDecl * decl = call->getCalleeDecl()->getAsFunction();
    if (!isFuncDeclProcessed(decl)) {
        funcdecls.push_back(decl);
    }
    return true;
}

bool ReorderFuncDeclsVisitor::compare(FunctionDecl * a, FunctionDecl * b) {
    auto a_name = a->getNameAsString();
    auto b_name = b->getNameAsString();
    return a_name < b_name;
}

void ReorderFuncDeclsVisitor::rewriteFunctions() {
    if (!test) {
        shuffle(funcdecls.begin(), funcdecls.end(), *gen);
    } else {
        sort(funcdecls.begin(), funcdecls.end(), compare);
    }
    for (auto & funcdecl : funcdecls) {
        SourceLocation newLineLoc = mainDecl->getEndLoc().getLocWithOffset(1);
        rewriter->ReplaceText(newLineLoc, "\n\n");
        SourceLocation funcDeclLoc = mainDecl->getEndLoc().getLocWithOffset(1);
        rewriter->ReplaceText(funcDeclLoc, funcdecl->getSourceRange());
        SourceRange funcDeclOldLoc = funcdecl->getBody()->getSourceRange();
        rewriter->ReplaceText(funcDeclOldLoc, ";");
    }
    rewriter->ReplaceText(mainDecl->getEndLoc().getLocWithOffset(1), "\n\n");
}

ReorderFuncDeclsASTConsumer::ReorderFuncDeclsASTConsumer(Rewriter * rewriter, mt19937 * gen, const bool test) :
                                                         visitor(rewriter, gen, test) {}

void ReorderFuncDeclsASTConsumer::HandleTranslationUnit(ASTContext &ctx) {
    visitor.TraverseDecl(ctx.getTranslationUnitDecl());
    visitor.rewriteFunctions();
}

ReorderFuncDeclsTransformation::ReorderFuncDeclsTransformation(float p, const int seed, const bool test) :
                                                               ITransformation(p, "reorder function decls"),
                                                               gen(new mt19937(seed)), test(test) {}

ReorderFuncDeclsTransformation::~ReorderFuncDeclsTransformation() {}

unique_ptr<ASTConsumer> ReorderFuncDeclsTransformation::getConsumer(Rewriter *rewriter) {
    return llvm::make_unique<ReorderFuncDeclsASTConsumer>(rewriter, gen, test);
}
