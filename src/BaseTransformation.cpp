#include "../include/BaseTransformation.h"

using clang::IfStmt;
using llvm::isa, llvm::cast;

BasicVisitor::BasicVisitor(Rewriter &rewriter) : rewriter(rewriter) {}

bool BasicVisitor::VisitStmt(Stmt *s) {
    // Only care about If statements.
    if (isa<IfStmt>(s)) {
        IfStmt *IfStatement = cast<IfStmt>(s);
        Stmt *Then = IfStatement->getThen();

        rewriter.InsertText(Then->getBeginLoc(), "// the 'if' part\n", true,
                               true);

        Stmt *Else = IfStatement->getElse();
        if (Else)
            rewriter.InsertText(Else->getBeginLoc(), "// the 'else' part\n",
                                   true, true);
    }

    return true;
}

unique_ptr<ASTConsumer> BaseTransformation::getConsumer(Rewriter &rewriter){
    return llvm::make_unique<BaseASTConsumer>(rewriter);
}

BaseASTConsumer::BaseASTConsumer(Rewriter &rewriter) : visitor(rewriter) {}

bool BaseASTConsumer::HandleTopLevelDecl(DeclGroupRef DR)  {
    for (auto b : DR) {
        // Traverse the declaration using our AST visitor.
        visitor.TraverseDecl(b);
        b->dump();
    }
    return true;
}