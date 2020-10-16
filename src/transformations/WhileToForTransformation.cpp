#include "../../include/transformations/WhileToForTransformation.h"

using clang::Expr, clang::ContinueStmt, clang::SourceRange;
using clang::Lexer, clang::CharSourceRange;
using llvm::isa, llvm::cast;
using std::unique_ptr, std::find, std::vector, std::string;

// ------------ WhileToForVisitor ------------

WhileToForVisitor::WhileToForVisitor(Rewriter * rewriter) :
        rewriter(rewriter), sm(rewriter->getSourceMgr()), opt(rewriter->getLangOpts()) {}

bool WhileToForVisitor::VisitWhileStmt(WhileStmt * whileStmt) {
    const Stmt * body = whileStmt->getBody();

    if (!body) {
        return true;
    }

    Expr * cond = whileStmt->getCond();
    string condText;
    if (cond) {
        SourceRange condRange = cond->getSourceRange();
        if (isa<clang::ValueStmt>(*cond->getExprStmt())) {
            condRange.setEnd(cond->getEndLoc().getLocWithOffset(1));
        }
        condText = Lexer::getSourceText(CharSourceRange::getCharRange(condRange), sm, opt).str();
    }
    auto whileText = "for ( ; " + condText + "; ) \n";
    ptrdiff_t diff = sm.getCharacterData(whileStmt->getBody()->getBeginLoc()) -
                      sm.getCharacterData(whileStmt->getBeginLoc());
    rewriter->ReplaceText(whileStmt->getBeginLoc(),
                          static_cast<unsigned int>(diff), whileText);
    return true;
}

// ------------ WhileToForASTConsumer ------------

WhileToForASTConsumer::WhileToForASTConsumer(Rewriter * rewriter) :
        visitor(rewriter) {}

bool WhileToForASTConsumer::HandleTopLevelDecl(DeclGroupRef DR) {
    for (clang::Decl * b : DR) {
        // Traverse each declaration in DeclGroup using our AST visitor.
        visitor.TraverseDecl(b);
        b->dump();
    }
    return true;
}

// ------------ WhileToForTransformation ------------

WhileToForTransformation::WhileToForTransformation(const float p) :
        ITransformation(p, "while to for") {}

unique_ptr<ASTConsumer> WhileToForTransformation::getConsumer(Rewriter * rewriter) {
    return std::make_unique<WhileToForASTConsumer>(rewriter);
}

ITransformation * WhileToForTransformation::buildFromConfig(const YAML::Node & config) {
    const auto p = config["p"].as<float>();
    return new WhileToForTransformation(p);
}
