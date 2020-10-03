#ifndef INCLUDE_TRANSFORMATIONS_REMOVECOMMENTSTRANSFORMATION_H_
#define INCLUDE_TRANSFORMATIONS_REMOVECOMMENTSTRANSFORMATION_H_

#include <yaml-cpp/yaml.h>

#include <string>
#include <vector>
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

using clang::ASTConsumer, clang::Rewriter, clang::RecursiveASTVisitor, clang::Decl,
clang::DeclGroupRef, clang::Stmt, clang::ASTContext;
using std::unique_ptr, std::vector, std::string;

class RemoveCommentsASTConsumer : public ASTConsumer {
 public:
    explicit RemoveCommentsASTConsumer(Rewriter * rewriter);
    void HandleTranslationUnit(ASTContext &ctx); // NOLINT.
 private:
    Rewriter * rewriter;
};

class RemoveCommentsTransformation : public ITransformation {
 public:
    explicit RemoveCommentsTransformation(const float p);
    ~RemoveCommentsTransformation() = default;
    unique_ptr<ASTConsumer> getConsumer(Rewriter *rewriter);
    static ITransformation * buildFromConfig(const YAML::Node & config);
};

#endif  // INCLUDE_TRANSFORMATIONS_REMOVECOMMENTSTRANSFORMATION_H_

