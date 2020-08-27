#include "../../include/transformations/AddCommentsTransformation.h"

using clang::IfStmt, clang::ForStmt, clang::WhileStmt;
using llvm::isa, llvm::cast;
using std::unique_ptr, std::find, std::vector, std::string;

// ------------ AddCommentsVisitor ------------

AddCommentsVisitor::AddCommentsVisitor(Rewriter * rewriter, const vector<string> * statements) :
                                       rewriter(rewriter), statements(statements) {}

bool AddCommentsVisitor::VisitStmt(Stmt *s) {
    if (isa<IfStmt>(s)) {
        auto IfStatement = cast<IfStmt>(s);
        if (containStatement(ifInside)) {
            rewriter->InsertText(IfStatement->getThen()->getBeginLoc(), "/* 'if' inside */\n",
                                 true, true);
        }
        if (containStatement(ifBegin)) {
            rewriter->InsertText(IfStatement->getBeginLoc(), "/* 'if' begin */\n",
                                 true, true);
        }
    } else if (isa<ForStmt>(s)) {
        auto ForStatement = cast<ForStmt>(s);
        if (containStatement(forInside)) {
            rewriter->InsertText(ForStatement->getBody()->getBeginLoc(), "/* 'for' inside */\n",
                                 true, true);
        }
        if (containStatement(forBegin)) {
            rewriter->InsertText(ForStatement->getBeginLoc(), "/* 'for' begin */\n",
                                 true, true);
        }
    } else if (isa<WhileStmt>(s)) {
        auto WhileStatement = cast<WhileStmt>(s);
        if (containStatement(whileInside)) {
            rewriter->InsertText(WhileStatement->getBody()->getBeginLoc(), "/* 'while' inside */\n",
                                 true, true);
        }
        if (containStatement(whileBegin)) {
            rewriter->InsertText(WhileStatement->getBeginLoc(), "/* 'while' begin */\n",
                                 true, true);
        }
    }
    return true;
}

bool AddCommentsVisitor::containStatement(string const &stmt) {
    return find(statements->begin(), statements->end(), stmt) != statements->end();
}

// ------------ AddCommentsASTConsumer ------------

AddCommentsASTConsumer::AddCommentsASTConsumer(Rewriter * rewriter, const vector<string> * statements) :
                                               visitor(rewriter, statements) {}

bool AddCommentsASTConsumer::HandleTopLevelDecl(DeclGroupRef DR) {
    for (clang::Decl * b : DR) {
        // Traverse each declaration in DeclGroup using our AST visitor.
        visitor.TraverseDecl(b);
        b->dump();
    }
    return true;
}

// ------------ AddCommentsTransformation ------------

AddCommentsTransformation::AddCommentsTransformation(float p, const vector<string> * statements) :
                                                     ITransformation(p, "add comments"),
                                                     statements(statements) {}

AddCommentsTransformation::~AddCommentsTransformation() {}

unique_ptr<ASTConsumer> AddCommentsTransformation::getConsumer(Rewriter * rewriter) {
    return llvm::make_unique<AddCommentsASTConsumer>(rewriter, statements);
}