#include "../../include/transformations/ReorderFuncDeclsTransformation.h"

using llvm::isa, llvm::cast;
using std::unique_ptr, std::find, std::vector, std::string;
using clang::FunctionDecl, clang::VarDecl, clang::DeclRefExpr;

ReorderFuncDeclsVisitor::ReorderFuncDeclsVisitor(Rewriter * rewriter, mt19937 * gen, const bool test) :
                                                 rewriter(rewriter), gen(gen), test(test) {}

bool ReorderFuncDeclsVisitor::VisitFunctionDecl(FunctionDecl * decl) {
    return true;
}

bool ReorderFuncDeclsVisitor::isFuncDeclProcessed(FunctionDecl * decl) {
    return find(funcdecls.begin(), funcdecls.end(), decl) != funcdecls.end();
}

bool ReorderFuncDeclsVisitor::VisitCallExpr(CallExpr * call) {
    FunctionDecl * decl = call->getCalleeDecl()->getAsFunction();
    if (decl->getName() != "main") {
        if (!isFuncDeclProcessed(decl)) {
            funcdecls.push_back(decl);
            rewriter->ReplaceText(decl->getBody()->getSourceRange(), ";");
        }
    }
    return true;
}

ReorderFuncDeclsASTConsumer::ReorderFuncDeclsASTConsumer(Rewriter * rewriter, mt19937 * gen, const bool test) :
                                                         visitor(rewriter, gen, test) {}

void ReorderFuncDeclsASTConsumer::HandleTranslationUnit(ASTContext &ctx) {
    visitor.TraverseDecl(ctx.getTranslationUnitDecl());
}

ReorderFuncDeclsTransformation::ReorderFuncDeclsTransformation(float p, const int seed, const bool test) :
                                                               ITransformation(p, "reorder function decls"),
                                                               gen(new mt19937(seed)), test(test) {}

ReorderFuncDeclsTransformation::~ReorderFuncDeclsTransformation() {}

unique_ptr<ASTConsumer> ReorderFuncDeclsTransformation::getConsumer(Rewriter *rewriter) {
    return llvm::make_unique<ReorderFuncDeclsASTConsumer>(rewriter, gen, test);
}
