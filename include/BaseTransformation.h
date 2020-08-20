#ifndef INCLUDE_BASETRANSFORMATION_H_
#define INCLUDE_BASETRANSFORMATION_H_

#include <random>
#include <iostream>
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

using clang::ASTFrontendAction, clang::ASTConsumer, clang::CompilerInstance,
    clang::StringRef, clang::Rewriter, clang::RecursiveASTVisitor,
    clang::DeclGroupRef, clang::ASTContext, clang::Stmt;
using llvm::make_unique;
using std::uniform_real_distribution, std::random_device, std::mt19937;
using std::unique_ptr, std::vector, std::string;

class BasicVisitor : public RecursiveASTVisitor<BasicVisitor> {
    /* RecursiveASTVisitor is a set of actions that are done
     * when a certain node of AST is reached */
 public:
    explicit BasicVisitor(Rewriter  * rewriter);
    bool VisitStmt(Stmt *s);
 private:
    Rewriter * rewriter;
};

class BaseASTConsumer : public ASTConsumer {
    /* ASTConsumer is an interface for reading an AST produced by the Clang parser. */
 public:
    explicit BaseASTConsumer(Rewriter * rewriter);
    bool HandleTopLevelDecl(DeclGroupRef DR);
 private:
    BasicVisitor visitor;
};

class BaseTransformation {
    /* Class for storing data about transformation specified in .yaml config */
 public:
    explicit BaseTransformation(float p) : p(p) {}
    static unique_ptr<ASTConsumer> getConsumer(Rewriter * rewriter);
    float p;
};

#endif  // INCLUDE_BASETRANSFORMATION_H_

