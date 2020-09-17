#include "../../include/transformations/IfElseSwapTransformation.h"

#include <iostream>

using clang::IfStmt, clang::ForStmt, clang::WhileStmt;
using clang::Lexer, clang::CharSourceRange;
using llvm::isa, llvm::cast;
using std::unique_ptr, std::find, std::vector, std::string;

// ------------ IfElseSwapVisitor ------------

IfElseSwapVisitor::IfElseSwapVisitor(Rewriter * rewriter) :
        rewriter(rewriter), sm(rewriter->getSourceMgr()), opt(rewriter->getLangOpts()) {}

bool IfElseSwapVisitor::VisitStmt(Stmt *s) {
    if (isa<IfStmt>(s)) {
        auto ifStmt = cast<IfStmt>(s);
        if (ifStmt->hasElseStorage()) {
            auto elseStmt = ifStmt->getElse();
            // Checking if "else if" visited
            if ((!isa<IfStmt>(elseStmt))) {
                if (isNotVisited(ifStmt)) {
                    // Rewriting if conditions, e.g condition "if(x == 0)" will be transformed to "if(!(x == 0))"
                    rewriter->InsertTextAfter(ifStmt->getCond()->getBeginLoc(), "!(");
                    rewriter->InsertTextAfter(ifStmt->getCond()->getEndLoc().getLocWithOffset(1), ")");
                    // Getting bodies of "if" and "else" statements
                    auto ifRange = ifStmt->getThen()->getSourceRange();
                    ifRange.setBegin(ifRange.getBegin());
                    ifRange.setEnd(ifRange.getEnd().getLocWithOffset(1));
                    auto ifBodyText = Lexer::getSourceText(CharSourceRange::getCharRange(ifRange), sm, opt);
                    std::cout << ifBodyText.str() << std::endl;
                    if (ifBodyText.str()[0] != '{') {
                        ifRange.setEnd(ifRange.getEnd().getLocWithOffset(1));
                        ifBodyText = Lexer::getSourceText(CharSourceRange::getCharRange(ifRange), sm, opt);
                    }
                    auto elseRange = elseStmt->getSourceRange();
                    elseRange.setBegin(elseRange.getBegin());
                    elseRange.setEnd(elseRange.getEnd().getLocWithOffset(1));
                    auto elseBodyText = Lexer::getSourceText(CharSourceRange::getCharRange(elseRange), sm, opt);
                    std::cout << elseBodyText.str() << std::endl;
                    if (elseBodyText.str()[0] != '{') {
                        elseRange.setEnd(elseRange.getEnd().getLocWithOffset(1));
                        elseBodyText = Lexer::getSourceText(CharSourceRange::getCharRange(elseRange), sm, opt);
                    }
                    rewriter->ReplaceText(elseRange, ifBodyText);
                    rewriter->ReplaceText(ifRange, elseBodyText);
                }
            } else {
                visitedIfStmt.push_back(cast<IfStmt>(elseStmt));
            }
        }
    }
    return true;
}

bool IfElseSwapVisitor::isNotVisited(IfStmt * s) {
    return find(visitedIfStmt.begin(), visitedIfStmt.end(), s) == visitedIfStmt.end();
}

// ------------ AddCommentsASTConsumer ------------

IfElseSwapASTConsumer::IfElseSwapASTConsumer(Rewriter * rewriter) :
        visitor(rewriter) {}

bool IfElseSwapASTConsumer::HandleTopLevelDecl(DeclGroupRef DR) {
    for (clang::Decl * b : DR) {
        // Traverse each declaration in DeclGroup using our AST visitor.
        visitor.TraverseDecl(b);
        b->dump();
    }
    return true;
}

// ------------ IfElseSwapTransformation ------------

IfElseSwapTransformation::IfElseSwapTransformation(const float p) :
        ITransformation(p, "if else swap") {}

unique_ptr<ASTConsumer> IfElseSwapTransformation::getConsumer(Rewriter * rewriter) {
    return llvm::make_unique<IfElseSwapASTConsumer>(rewriter);
}

ITransformation * IfElseSwapTransformation::buildFromConfig(const YAML::Node & config) {
    const auto p = config["p"].as<float>();
    return new IfElseSwapTransformation(p);
}
