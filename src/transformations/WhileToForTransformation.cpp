#include "../../include/transformations/WhileToForTransformation.h"

using clang::Expr, clang::ContinueStmt, clang::SourceRange;
using clang::Lexer, clang::CharSourceRange;
using llvm::isa, llvm::cast;
using std::unique_ptr, std::find, std::vector, std::string;

// ------------ WhileToForVisitor ------------

WhileToForVisitor::WhileToForVisitor(Rewriter * rewriter) :
        rewriter(rewriter), sm(rewriter->getSourceMgr()), opt(rewriter->getLangOpts()) {}

bool WhileToForVisitor::VisitWhileStmt(WhileStmt * whileStmt) {
    auto loc = whileStmt->getBeginLoc();
    if (sm.isWrittenInMainFile(loc)) {
        const Stmt *body = whileStmt->getBody();

        if (!body) {
            return true;
        }

        Expr *cond = whileStmt->getCond();
        string condText;
        if (cond) {
            SourceRange condRange = cond->getSourceRange();
            if (isa<clang::ValueStmt>(*cond->getExprStmt())) {
                condRange.setEnd(Lexer::getLocForEndOfToken(cond->getEndLoc(), 1, sm, opt).getLocWithOffset(1));
            }
            condText = Lexer::getSourceText(CharSourceRange::getCharRange(condRange), sm, opt).str();
        }
        auto whileText = "for ( ; " + condText + "; ) \n";
        ptrdiff_t diff = sm.getCharacterData(whileStmt->getBody()->getBeginLoc()) -
                         sm.getCharacterData(whileStmt->getBeginLoc());
        rewriter->ReplaceText(whileStmt->getBeginLoc(),
                              static_cast<unsigned int>(diff), whileText);
    }
    return true;
}

// ------------ WhileToForASTConsumer ------------

WhileToForASTConsumer::WhileToForASTConsumer(Rewriter * rewriter) :
        visitor(rewriter), rewriter(rewriter) {}

void WhileToForASTConsumer::HandleTranslationUnit(ASTContext &ctx) {
    visitor.TraverseDecl(ctx.getTranslationUnitDecl());
    rewriter->overwriteChangedFiles();
}

// ------------ WhileToForTransformation ------------

WhileToForTransformation::WhileToForTransformation(const float p) :
        ITransformation(p, "while to for") {}

unique_ptr<ASTConsumer> WhileToForTransformation::getConsumer(Rewriter * rewriter) {
    return llvm::make_unique<WhileToForASTConsumer>(rewriter);
}

ITransformation * WhileToForTransformation::buildFromConfig(const YAML::Node & config) {
    const auto p = config["p"].as<float>();
    return new WhileToForTransformation(p);
}
