#include "../../include/transformations/AddCommentsTransformation.h"

using clang::IfStmt;
using llvm::isa, llvm::cast;
using std::unique_ptr;

// ------------ AddCommentsVisitor ------------

AddCommentsVisitor::AddCommentsVisitor(Rewriter * rewriter) : rewriter(rewriter) {}

bool AddCommentsVisitor::VisitStmt(Stmt *s) {
    // When the "if" statement is reached we insert a comment
    if (isa<IfStmt>(s)) {
        auto IfStatement = cast<IfStmt>(s);
        Stmt *Then = IfStatement->getThen();

        rewriter->InsertText(Then->getBeginLoc(), "// the 'if' part\n",
                             true, true);
    }
    return true;
}

// ------------ AddCommentsASTConsumer ------------

AddCommentsASTConsumer::AddCommentsASTConsumer(Rewriter * rewriter) : visitor(rewriter) {}

bool AddCommentsASTConsumer::HandleTopLevelDecl(DeclGroupRef DR) {
    for (auto b : DR) {
        // Traverse the declaration using our AST visitor.
        visitor.TraverseDecl(b);
        b->dump();
    }
    return true;
}

// ------------ AddCommentsTransformation ------------

AddCommentsTransformation::AddCommentsTransformation(float p) : ITransformation(p) {}

AddCommentsTransformation::~AddCommentsTransformation() {}

unique_ptr<ASTConsumer> AddCommentsTransformation::getConsumer(Rewriter * rewriter) {
    return llvm::make_unique<AddCommentsASTConsumer>(rewriter);
}