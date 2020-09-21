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

using clang::ASTConsumer, clang::Rewriter, clang::RecursiveASTVisitor, clang::IfStmt,
clang::DeclGroupRef, clang::Stmt, clang::SourceManager, clang::LangOptions, clang::SourceRange;
using std::unique_ptr, std::vector, std::string;

/// RecursiveASTVisitor is a set of actions that are done
/// when a certain node of AST is reached
class IfElseSwapVisitor : public RecursiveASTVisitor<IfElseSwapVisitor> {
 public:
    explicit IfElseSwapVisitor(Rewriter * rewriter);
    /**
     * This function is called a certain clang::Stmt is visited
     */
    bool VisitStmt(Stmt *s);
 private:
    Rewriter * rewriter;
    SourceManager & sm;
    LangOptions opt;
    vector<IfStmt *> visitedIfStmt;
    bool isNotVisited(IfStmt * s);
    void swapBodies(IfStmt * ifStmt);
    void rewriteCondition(IfStmt * ifStmt);
    void processIfStmt(IfStmt * ifStmt);
    string getBodyAsString(SourceRange * range);
};

class IfElseSwapASTConsumer : public ASTConsumer {
 public:
    explicit IfElseSwapASTConsumer(Rewriter * rewriter);
    /**
     * HandleTopLevelDecl handles all the declaration (or definition),
     * e.g. a variable, typedef, function, struct, etc
     * @param DR     Iterating through DeclGroupRef we are getting all the declarations
     *               belongs to current DeclGroup
     * @return       true to continue parsing, or false to abort parsing.
     */
    bool HandleTopLevelDecl(DeclGroupRef DR);
 private:
    IfElseSwapVisitor visitor;
};

class IfElseSwapTransformation : public ITransformation {
 public:
    explicit IfElseSwapTransformation(const float p);
    ~IfElseSwapTransformation() = default;
    unique_ptr<ASTConsumer> getConsumer(Rewriter *rewriter);
    static ITransformation * buildFromConfig(const YAML::Node & config);
};

#endif  // INCLUDE_TRANSFORMATIONS_IFELSESWAPTRANSFORMATION_H_
