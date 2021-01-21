#include "../../include/transformations/ForToWhileTransformation.h"
#include <iostream>
using clang::Expr, clang::ContinueStmt, clang::ValueStmt, clang::CompoundStmt;
using clang::Lexer, clang::CharSourceRange, clang::SourceLocation;
using llvm::isa, llvm::cast;
using std::unique_ptr, std::find, std::vector, std::string;

// ------------ ForToWhileVisitor ------------

ForToWhileVisitor::ForToWhileVisitor(Rewriter * rewriter) :
        rewriter(rewriter), sm(rewriter->getSourceMgr()), opt(rewriter->getLangOpts()) {}

string ForToWhileVisitor::getTextFromRange(SourceRange range) {
    return Lexer::getSourceText(CharSourceRange::getCharRange(range), sm, opt).str();
}

bool ForToWhileVisitor::VisitForStmt(ForStmt * forStmt) {
    auto loc = forStmt->getBeginLoc();
    if (sm.isWrittenInMainFile(loc)) {
        const Stmt *body = forStmt->getBody();

        if (!body) {
            return true;
        }

        processInit(forStmt);
        processCond(forStmt);
        processInc(forStmt);
        processBody(forStmt);
    }
    return true;
}

void ForToWhileVisitor::processBody(ForStmt *forStmt) {
    const Stmt * body = forStmt->getBody();
    // If the inc part of for is empty, then it is not necessary to add brackets
    if (forStmt->getInc()) {
        if (!isa<clang::CompoundStmt>(body)) {
            rewriter->InsertText(body->getBeginLoc(), "{\n", true, true);
            rewriter->InsertText(Lexer::getLocForEndOfToken(body->getEndLoc(), 1, sm, opt).getLocWithOffset(2),
                                 "}", true, true);
        }
    }
}

void ForToWhileVisitor::processInit(ForStmt * forStmt) {
    Stmt * init = forStmt->getInit();
    if (init) {
        SourceRange initRange = init->getSourceRange();
        if (isa<clang::ValueStmt>(*init)) {
            initRange.setEnd(Lexer::getLocForEndOfToken(init->getEndLoc(), 1, sm, opt).getLocWithOffset(1));
        }
        auto initText = "{\n" + getTextFromRange(initRange) + ";\n";
        rewriter->InsertText(forStmt->getBeginLoc(), initText, true, true);
    }
}

void ForToWhileVisitor::processCond(ForStmt * forStmt) {
    Expr * cond = forStmt->getCond();
    string condText;
    if (cond) {
        SourceRange condRange = cond->getSourceRange();
        if (isa<ValueStmt>(*cond->getExprStmt())) {
            condRange.setEnd(Lexer::getLocForEndOfToken(cond->getEndLoc(), 1, sm, opt).getLocWithOffset(1));
        }
        condText = getTextFromRange(condRange);
    } else {
        condText = "1";
    }
    auto whileText = "while (" + condText + ") ";
    ptrdiff_t diff = sm.getCharacterData(forStmt->getBody()->getBeginLoc()) -
                     sm.getCharacterData(forStmt->getBeginLoc());
    rewriter->ReplaceText(forStmt->getBeginLoc(),
                          static_cast<unsigned int>(diff), whileText);
}

void ForToWhileVisitor::processInc(ForStmt * forStmt) {
    Expr * inc = forStmt->getInc();
    if (!inc) {
        return;
    } else {
        SourceRange incRange = inc->getSourceRange();
        if (isa<clang::ValueStmt>(*inc->getExprStmt())) {
            incRange.setEnd(Lexer::getLocForEndOfToken(inc->getEndLoc(), 1, sm, opt).getLocWithOffset(1));
        }
        auto incText = getTextFromRange(incRange) + "; ";
        vector<const ContinueStmt *> continues;
        collectContinues(forStmt->getBody(), &continues);
        for (auto countinue_stmt : continues) {
            rewriter->InsertText(countinue_stmt->getBeginLoc(), incText, true, true);
        }
        incText += "\n";
        if (forStmt->getInit()) {
            incText += "\n}";
        }
        const Stmt * body = forStmt->getBody();
        SourceLocation forEndLoc;
        if (!isa<CompoundStmt>(body)) {
            forEndLoc = Lexer::getLocForEndOfToken(forStmt->getEndLoc(), 0, sm, opt).getLocWithOffset(1);
            incText = "\n" + incText;
            rewritingStack.push(pair(forEndLoc, incText));
            if (!isa<ForStmt>(body)) {
                pair<SourceLocation, string> pair_;
                while (!rewritingStack.empty()) {
                    pair_ = rewritingStack.top();
                    rewriter->InsertText(pair_.first, pair_.second, true, true);
                    rewritingStack.pop();
                }
            }
        } else {
            forEndLoc = cast<CompoundStmt>(body)->getRBracLoc();
            rewriter->InsertText(forEndLoc, "\t" + incText , true, true);
        }
    }
}

void ForToWhileVisitor::collectContinues(const Stmt * s, vector<const ContinueStmt *> *continues) {
    if (const ContinueStmt * dec = clang::dyn_cast<ContinueStmt>(s)) {
        continues->push_back(dec);
    }
    const auto &children = s->children();
    for (const clang::Stmt *child : children) {
        if (child) {
            collectContinues(child, continues);
        }
    }
}

// ------------ ForToWhileASTConsumer ------------

ForToWhileASTConsumer::ForToWhileASTConsumer(Rewriter * rewriter) :
        visitor(rewriter), rewriter(rewriter) {}

void ForToWhileASTConsumer::HandleTranslationUnit(ASTContext &ctx) {
    visitor.TraverseDecl(ctx.getTranslationUnitDecl());
    rewriter->overwriteChangedFiles();
}

// ------------ ForToWhileTransformation ------------

ForToWhileTransformation::ForToWhileTransformation(const float p) :
        ITransformation(p, "for to while") {}

unique_ptr<ASTConsumer> ForToWhileTransformation::getConsumer(Rewriter * rewriter) {
    return llvm::make_unique<ForToWhileASTConsumer>(rewriter);
}

ITransformation * ForToWhileTransformation::buildFromConfig(const YAML::Node & config) {
    const auto p = config["p"].as<float>();
    return new ForToWhileTransformation(p);
}
