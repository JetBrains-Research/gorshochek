#include "../../include/transformations/IfElseSwapTransformation.h"
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
using clang::IfStmt, clang::ForStmt, clang::WhileStmt;
using clang::Lexer, clang::CharSourceRange;
using llvm::isa, llvm::cast;
using std::unique_ptr, std::find, std::vector, std::string;

// ------------ IfElseSwapVisitor ------------

IfElseSwapVisitor::IfElseSwapVisitor(Rewriter * rewriter) :
        rewriter(rewriter), sm(rewriter->getSourceMgr()), opt(rewriter->getLangOpts()) {}

void IfElseSwapVisitor::rewriteCondition(IfStmt * ifStmt) {
    // Rewriting if conditions, e.g condition "if(x == 0)" will be transformed to "if(!(x == 0))"
    auto range = ifStmt->getCond()->getSourceRange();
    range.setEnd(Lexer::getLocForEndOfToken(range.getEnd(), 1, sm, opt).getLocWithOffset(1));
    auto condString = Lexer::getSourceText(CharSourceRange::getCharRange(range), sm, opt).str();
    if (!condString.empty()) {
        rewriter->InsertTextAfter(range.getBegin(), "!(");
        rewriter->InsertTextAfter(range.getEnd(), ")");
    }
}

string IfElseSwapVisitor::getBodyAsString(SourceRange * range) {
    range->setBegin(range->getBegin());
    range->setEnd(Lexer::getLocForEndOfToken(range->getEnd(), 1, sm, opt).getLocWithOffset(1));
    auto text = Lexer::getSourceText(CharSourceRange::getCharRange(*range), sm, opt);
    // If no brackets found then we add a small offset
    if (text.str()[0] != '{') {
        range->setEnd(range->getEnd().getLocWithOffset(1));
        text = Lexer::getSourceText(CharSourceRange::getCharRange(*range), sm, opt);
    }
    return text;
}

void IfElseSwapVisitor::swapBodies(IfStmt * ifStmt) {
    // Getting bodies of "if" and "else" statements
    auto ifRange = ifStmt->getThen()->getSourceRange();
    auto ifBodyText = getBodyAsString(&ifRange);
    auto elseRange = ifStmt->getElse()->getSourceRange();
    auto elseBodyText = getBodyAsString(&elseRange);
    // Swap if and else parts
    rewriter->ReplaceText(elseRange, ifBodyText);
    rewriter->ReplaceText(ifRange, elseBodyText);
}

void IfElseSwapVisitor::processIfStmt(IfStmt * ifStmt) {
    auto elseStmt = ifStmt->getElse();
    // Checking if visitor come to "else if"
    if (!isa<IfStmt>(elseStmt)) {
        // Checking if visitor come to "else" part of "else if"
        if (!isElseStmtOfVisited(ifStmt) && !isChildOfVisited(ifStmt)) {
            rewriteCondition(ifStmt);
            swapBodies(ifStmt);
        }
    }
    visitedIfStmt.push_back(ifStmt);
}

bool IfElseSwapVisitor::VisitIfStmt(IfStmt * ifStmt) {
    auto loc = ifStmt->getBeginLoc();
    if (sm.isWrittenInMainFile(loc)) {
        if (ifStmt->hasElseStorage() && !ifStmt->hasVarStorage()) {
            processIfStmt(ifStmt);
        }
    }
    return true;
}

bool IfElseSwapVisitor::isElseStmtOfVisited(IfStmt * ifStmt) {
    for (auto stmt : visitedIfStmt) {
        if (ifStmt == stmt->getElse()) {
            return true;
        }
    }
    return false;
}

bool IfElseSwapVisitor::isChild(Stmt * root, Stmt * leaf) {
    auto checker = ChildStmtChecker(leaf);
    checker.TraverseStmt(root);
    return checker.isChild();
}

bool IfElseSwapVisitor::isChildOfVisited(IfStmt * ifStmt) {
    Stmt * elseStmt;
    Stmt * thenStmt;
    IfStmt * stmt;
    IfStmt * prev;

    for (auto it = visitedIfStmt.rbegin(); it != visitedIfStmt.rend(); ++it) {
        stmt = *it;
        prev = *(it + 1);
        thenStmt = stmt->getThen();
        elseStmt = stmt->getElse();
        // If in AST children of thenStmt consists ifStmt

        if (isChild(thenStmt, ifStmt)) {
            if (it + 1 != visitedIfStmt.rend()) {
                return !(prev->getElse() == stmt);
            } else {
                return false;
            }
        }
        // If else part of current stmt is "else if" then change elseStmt to body of "else if"
        elseStmt = isa<IfStmt>(elseStmt) ? cast<IfStmt>(elseStmt)->getThen() : elseStmt;
        // If in AST children of elseStmt consists ifStmt return
        // "false" if current stmt has "else if", "true" otherwise
        if (isChild(elseStmt, ifStmt)) {
            if (it + 1 != visitedIfStmt.rend()) {
                return !(prev->getElse() == stmt);
            } else {
                return false;
            }
        }
    }
    return false;
}

// ------------ ChildStmtChecker ------------

ChildStmtChecker::ChildStmtChecker(const Stmt * leaf) : leaf(leaf) {}

bool ChildStmtChecker::VisitStmt(Stmt * stmt) {
    isChild_ = isChild_ || (stmt == leaf);
    return true;
}

bool ChildStmtChecker::isChild() {
    return isChild_;
}

// ------------ AddCommentsASTConsumer ------------

IfElseSwapASTConsumer::IfElseSwapASTConsumer(Rewriter * rewriter) :
        visitor(rewriter), rewriter(rewriter) {}

void IfElseSwapASTConsumer::HandleTranslationUnit(ASTContext &ctx) {
    visitor.TraverseDecl(ctx.getTranslationUnitDecl());
    rewriter->overwriteChangedFiles();
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
