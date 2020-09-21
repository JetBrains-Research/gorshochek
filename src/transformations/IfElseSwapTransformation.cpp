#include "../../include/transformations/IfElseSwapTransformation.h"

#include <iostream>

using clang::IfStmt, clang::ForStmt, clang::WhileStmt;
using clang::Lexer, clang::CharSourceRange;
using llvm::isa, llvm::cast;
using std::unique_ptr, std::find, std::vector, std::string;

// ------------ IfElseSwapVisitor ------------

IfElseSwapVisitor::IfElseSwapVisitor(Rewriter * rewriter) :
        rewriter(rewriter), sm(rewriter->getSourceMgr()), opt(rewriter->getLangOpts()) {}

void IfElseSwapVisitor::rewriteCondition(IfStmt * ifStmt) {
    // Rewriting if conditions, e.g condition "if(x == 0)" will be transformed to "if(!(x == 0))"
    rewriter->InsertTextAfter(ifStmt->getCond()->getBeginLoc(), "!(");
    rewriter->InsertTextAfter(ifStmt->getCond()->getEndLoc().getLocWithOffset(1), ")");
}

string IfElseSwapVisitor::getBodyAsString(SourceRange * range) {
    range->setBegin(range->getBegin());
    range->setEnd(range->getEnd().getLocWithOffset(1));
    auto text = Lexer::getSourceText(CharSourceRange::getCharRange(*range), sm, opt);
    // If no brackets found then we add a small offset
    if (text.str()[0] != '{') {
        range->setEnd(range->getEnd().getLocWithOffset(1));
        text = Lexer::getSourceText(CharSourceRange::getCharRange(*range), sm, opt);
    }
    return text;
}

void IfElseSwapVisitor::swapBodies(IfStmt * ifStmt) {
    auto elseStmt = ifStmt->getElse();
    // Getting bodies of "if" and "else" statements
    auto ifRange = ifStmt->getThen()->getSourceRange();
    auto ifBodyText = getBodyAsString(&ifRange);
    auto elseRange = elseStmt->getSourceRange();
    auto elseBodyText = getBodyAsString(&elseRange);
    // Swap if and else parts
    rewriter->ReplaceText(elseRange, ifBodyText);
    rewriter->ReplaceText(ifRange, elseBodyText);
}

bool IfElseSwapVisitor::VisitStmt(Stmt *s) {
    if (isa<IfStmt>(s)) {
        auto ifStmt = cast<IfStmt>(s);
        if (ifStmt->hasElseStorage()) {
            auto elseStmt = ifStmt->getElse();
            // Checking if visitor come to "else if"
            if ((!isa<IfStmt>(elseStmt))) {
                // Checking if visitor come to "else" part of "else if"
                if (isNotVisited(ifStmt)) {
                    rewriteCondition(ifStmt);
                    swapBodies(ifStmt);
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
