#ifndef INCLUDE_TRANSFORMATIONS_IFELSESWAPTRANSFORMATION_H_
#define INCLUDE_TRANSFORMATIONS_IFELSESWAPTRANSFORMATION_H_

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

using clang::ASTConsumer, clang::Rewriter, clang::RecursiveASTVisitor, clang::IfStmt, clang::ConstStmtVisitor,
clang::ASTContext, clang::Stmt, clang::SourceManager, clang::LangOptions, clang::SourceRange;
using std::unique_ptr, std::vector, std::string;

/// RecursiveASTVisitor is a set of actions that are done
/// when a certain node of AST is reached
class IfElseSwapVisitor : public RecursiveASTVisitor<IfElseSwapVisitor> {
 public:
    explicit IfElseSwapVisitor(Rewriter * rewriter);
    /**
     * This function is called a certain clang::Stmt is visited
     */
    bool VisitIfStmt(IfStmt * ifStmt);
 private:
    Rewriter * rewriter;
    SourceManager & sm;
    LangOptions opt;
    vector<IfStmt *> visitedIfStmt;
    bool isElseStmtOfVisited(IfStmt * ifStmt);
    bool isChildOfVisited(IfStmt * ifStmt);
    bool isChild(Stmt * root, Stmt * leaf);
    void swapBodies(IfStmt * ifStmt);
    void rewriteCondition(IfStmt * ifStmt);
    void processIfStmt(IfStmt * ifStmt);
    string getBodyAsString(SourceRange * range);
};

class ChildFinder : public RecursiveASTVisitor<ChildFinder> {
public:
    ChildFinder(const Stmt * leaf);
    bool VisitStmt(Stmt * stmt);
    bool isChild();
private:
    const Stmt * leaf;
    bool isChild_ = false;
};

class IfElseSwapASTConsumer : public ASTConsumer {
 public:
    explicit IfElseSwapASTConsumer(Rewriter * rewriter);
    void HandleTranslationUnit(ASTContext &ctx); // NOLINT.
 private:
    IfElseSwapVisitor visitor;
    Rewriter * rewriter;
};

class IfElseSwapTransformation : public ITransformation {
 public:
    explicit IfElseSwapTransformation(const float p);
    ~IfElseSwapTransformation() = default;
    unique_ptr<ASTConsumer> getConsumer(Rewriter *rewriter);
    static ITransformation * buildFromConfig(const YAML::Node & config);
};

#endif  // INCLUDE_TRANSFORMATIONS_IFELSESWAPTRANSFORMATION_H_
