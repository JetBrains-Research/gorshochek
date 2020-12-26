#ifndef INCLUDE_TRANSFORMATIONS_OPENMACROSTRANSFORMATION_H_
#define INCLUDE_TRANSFORMATIONS_OPENMACROSTRANSFORMATION_H_

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

using clang::ASTConsumer, clang::Rewriter, clang::RecursiveASTVisitor,
clang::ASTContext, clang::SourceManager, clang::LangOptions,
clang::ContinueStmt, clang::FunctionDecl, clang::SourceRange;
using std::unique_ptr, std::vector, std::string;

/// RecursiveASTVisitor is a set of actions that are done
/// when a certain node of AST is reached
class OpenMacrosVisitor : public RecursiveASTVisitor<OpenMacrosVisitor> {
 public:
    explicit OpenMacrosVisitor(Rewriter * rewriter);
    /**
     * This function is called a certain clang::Decl is visited
     */
    bool VisitFunctionDecl(FunctionDecl * func);
 private:
    Rewriter * rewriter;
    SourceManager & sm;
    LangOptions opt;

    bool visited;
};

class OpenMacrosASTConsumer : public ASTConsumer {
 public:
    explicit OpenMacrosASTConsumer(Rewriter * rewriter);
    void HandleTranslationUnit(ASTContext &ctx); // NOLINT
 private:
    OpenMacrosVisitor visitor;
    Rewriter * rewriter;
};

class OpenMacrosTransformation : public ITransformation {
 public:
    explicit OpenMacrosTransformation(const float p);
    ~OpenMacrosTransformation() = default;
    unique_ptr<ASTConsumer> getConsumer(Rewriter *rewriter);
    static ITransformation * buildFromConfig(const YAML::Node & config);
};

#endif  // INCLUDE_TRANSFORMATIONS_OPENMACROSTRANSFORMATION_H_
