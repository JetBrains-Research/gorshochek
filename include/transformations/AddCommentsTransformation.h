#ifndef INCLUDE_TRANSFORMATIONS_ADDCOMMENTSTRANSFORMATION_H_
#define INCLUDE_TRANSFORMATIONS_ADDCOMMENTSTRANSFORMATION_H_

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

using clang::ASTConsumer, clang::Rewriter, clang::RecursiveASTVisitor, clang::ASTContext,
clang::DeclGroupRef, clang::Stmt, clang::SourceManager;
using std::unique_ptr, std::vector, std::string;

/// RecursiveASTVisitor is a set of actions that are done
/// when a certain node of AST is reached
class AddCommentsVisitor : public RecursiveASTVisitor<AddCommentsVisitor> {
 public:
    explicit AddCommentsVisitor(Rewriter * rewriter, const vector<string> * statements);
    /**
     * This function is called a certain clang::Stmt is visited
     */
    bool VisitStmt(Stmt *s);
 private:
    Rewriter * rewriter;
    SourceManager & sm;
    const vector<string> * statements;
    /**
     * Check if vector statements contains a specific statement
     * @param stmt   Statement to be checked
     * @return
     */
    bool containStatement(string const &stmt);

    const string ifBegin = "ifBegin";
    const string ifInside = "ifInside";
    const string forBegin = "forBegin";
    const string forInside = "forInside";
    const string whileBegin = "whileBegin";
    const string whileInside = "whileInside";
};

class AddCommentsASTConsumer : public ASTConsumer {
 public:
    explicit AddCommentsASTConsumer(Rewriter * rewriter, const vector<string> * statements);
    void HandleTranslationUnit(ASTContext &ctx); // NOLINT
 private:
    AddCommentsVisitor visitor;
    Rewriter * rewriter;
};

class AddCommentsTransformation : public ITransformation {
 public:
    explicit AddCommentsTransformation(const float p, const vector<string> * statements);
    ~AddCommentsTransformation() = default;
    unique_ptr<ASTConsumer> getConsumer(Rewriter *rewriter);
    static ITransformation * buildFromConfig(const YAML::Node & config);

 private:
    /**
     * Along with probability, the AddCommentsTransformation class constructor also accepts
     * statements which contains places where a comment should be inserted,
     * e.g ["forBegin", "ifInside"] will add specific comment to the body of each "if"
     * and before each "for"
     */
    const vector<string> * statements;
};

#endif  // INCLUDE_TRANSFORMATIONS_ADDCOMMENTSTRANSFORMATION_H_
