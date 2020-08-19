#ifndef GORSHOCHEK_BASETRANSFORMATION_H
#define GORSHOCHEK_BASETRANSFORMATION_H

#include <iostream>
#include <random>
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

using std::unique_ptr, std::vector, std::string;
using std::uniform_real_distribution, std::random_device, std::mt19937;
using clang::ASTFrontendAction, \
      clang::ASTConsumer, \
      clang::CompilerInstance, \
      clang::StringRef, \
      clang::Rewriter, \
      clang::RecursiveASTVisitor, \
      clang::DeclGroupRef, \
      clang::ASTContext, \
      clang::Stmt;
using llvm::make_unique;

class BasicVisitor : public RecursiveASTVisitor<BasicVisitor> {
public:
    explicit BasicVisitor(Rewriter &rewriter);
    bool VisitStmt(Stmt *s);
private:
    Rewriter &rewriter;
};

// ASTConsumer is an interface for reading an AST produced by the Clang parser.
class BaseASTConsumer : public ASTConsumer {
public:
    explicit BaseASTConsumer(Rewriter &rewriter);
    bool HandleTopLevelDecl(DeclGroupRef DR);
private:
    BasicVisitor visitor;
};

class BaseTransformation{
public:
    explicit BaseTransformation(float p): p(p) {};
    unique_ptr<ASTConsumer> getConsumer(Rewriter &rewriter);
    float p;
};

#endif //GORSHOCHEK_BASETRANSFORMATION_H
