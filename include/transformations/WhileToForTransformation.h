#ifndef INCLUDE_TRANSFORMATIONS_WHILETOFORTRANSFORMATION_H_
#define INCLUDE_TRANSFORMATIONS_WHILETOFORTRANSFORMATION_H_

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
clang::ASTContext, clang::WhileStmt, clang::SourceManager, clang::LangOptions,
clang::ContinueStmt, clang::Stmt;
using std::unique_ptr, std::vector, std::string;

/// RecursiveASTVisitor is a set of actions that are done
/// when a certain node of AST is reached
class WhileToForVisitor : public RecursiveASTVisitor<WhileToForVisitor> {
 public:
    explicit WhileToForVisitor(Rewriter * rewriter);
    /**
     * This function is called a certain clang::WhilrStmt is visited
     */
    bool VisitWhileStmt(WhileStmt * forStmt);
 private:
    Rewriter * rewriter;
    SourceManager & sm;
    LangOptions opt;
};

class WhileToForASTConsumer : public ASTConsumer {
 public:
    explicit WhileToForASTConsumer(Rewriter * rewriter);
    void HandleTranslationUnit(ASTContext &ctx); // NOLINT
 private:
    WhileToForVisitor visitor;
};

class WhileToForTransformation : public ITransformation {
 public:
    explicit WhileToForTransformation(const float p);
    ~WhileToForTransformation() = default;
    unique_ptr<ASTConsumer> getConsumer(Rewriter *rewriter);
    static ITransformation * buildFromConfig(const YAML::Node & config);
};

#endif   // INCLUDE_TRANSFORMATIONS_WHILETOFORTRANSFORMATION_H_
