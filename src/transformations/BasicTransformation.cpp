#include "../../include/transformations/BasicTransformation.h"

using clang::IfStmt;
using llvm::isa, llvm::cast;
using std::unique_ptr;

// ------------ BasicVisitor ------------

BasicVisitor::BasicVisitor(Rewriter * rewriter) : rewriter(rewriter) {}

bool BasicVisitor::VisitStmt(Stmt *s) {
    // When the "if" statement is reached we insert a comment
    if (isa<IfStmt>(s)) {
        auto IfStatement = cast<IfStmt>(s);
        Stmt *Then = IfStatement->getThen();

        rewriter->InsertText(Then->getBeginLoc(), "// the 'if' part\n",
                             true, true);
    }
    return true;
}

// ------------ BasicASTConsumer ------------

BasicASTConsumer::BasicASTConsumer(Rewriter * rewriter) : visitor(rewriter) {}

bool BasicASTConsumer::HandleTopLevelDecl(DeclGroupRef DR) {
    for (auto b : DR) {
        // Traverse the declaration using our AST visitor.
        visitor.TraverseDecl(b);
        b->dump();
    }
    return true;
}

// ------------ BasicTransformation ------------

BasicTransformation::BasicTransformation(float p) : ITransformation(p) {}

BasicTransformation::~BasicTransformation() {}

unique_ptr<ASTConsumer> BasicTransformation::getConsumer(Rewriter * rewriter) {
    return llvm::make_unique<BasicASTConsumer>(rewriter);
}
