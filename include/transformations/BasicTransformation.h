#ifndef INCLUDE_TRANSFORMATIONS_BASICTRANSFORMATION_H_
#define INCLUDE_TRANSFORMATIONS_BASICTRANSFORMATION_H_

#include <string>
#include <memory>

#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"

#include "../ITransformation.h"

using clang::ASTConsumer, clang::Rewriter, clang::RecursiveASTVisitor,
clang::DeclGroupRef, clang::Stmt;
using std::unique_ptr;

class BasicVisitor : public RecursiveASTVisitor<BasicVisitor> {
    /* RecursiveASTVisitor is a set of actions that are done
     * when a certain node of AST is reached */
 public:
    explicit BasicVisitor(Rewriter  * rewriter);
    bool VisitStmt(Stmt *s);
 private:
    Rewriter * rewriter;
};

class BasicASTConsumer : public ASTConsumer {
    /* ASTConsumer is an interface for reading an AST produced by the Clang parser. */
 public:
    explicit BasicASTConsumer(Rewriter * rewriter);
    bool HandleTopLevelDecl(DeclGroupRef DR);
 private:
    BasicVisitor visitor;
};

class BasicTransformation : public ITransformation {
 public:
    explicit BasicTransformation(float p);
    ~BasicTransformation();
    unique_ptr<ASTConsumer> getConsumer(Rewriter *rewriter);
};


#endif  // INCLUDE_TRANSFORMATIONS_BASICTRANSFORMATION_H_
