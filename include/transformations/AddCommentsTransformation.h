#ifndef INCLUDE_TRANSFORMATIONS_ADDCOMMENTSTRANSFORMATION_H_
#define INCLUDE_TRANSFORMATIONS_ADDCOMMENTSTRANSFORMATION_H_


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
clang::DeclGroupRef, clang::Stmt;
using std::unique_ptr, std::vector, std::string;

class AddCommentsVisitor : public RecursiveASTVisitor<AddCommentsVisitor> {
    /* RecursiveASTVisitor is a set of actions that are done
     * when a certain node of AST is reached */
 public:
    explicit AddCommentsVisitor(Rewriter * rewriter, vector<string> * statements);
    bool VisitStmt(Stmt *s);
 private:
    Rewriter * rewriter;
    vector<string> * statements;
    bool containStatement(string const &stmt);

    const string ifBegin = "ifBegin";
    const string ifInside = "ifInside";
    const string forBegin = "forBegin";
    const string forInside = "forInside";
    const string whileBegin = "whileBegin";
    const string whileInside = "whileInside";
};

class AddCommentsASTConsumer : public ASTConsumer {
    /* ASTConsumer is an interface for reading an AST produced by the Clang parser. */
 public:
    explicit AddCommentsASTConsumer(Rewriter * rewriter, vector<string> * statements);
    bool HandleTopLevelDecl(DeclGroupRef DR);
 private:
    AddCommentsVisitor visitor;
};

class AddCommentsTransformation : public ITransformation {
 public:
    explicit AddCommentsTransformation(float p, vector<string> * statements);
    ~AddCommentsTransformation();
    unique_ptr<ASTConsumer> getConsumer(Rewriter *rewriter);
 private:
    vector<string> * statements;
};

#endif  // INCLUDE_TRANSFORMATIONS_ADDCOMMENTSTRANSFORMATION_H_
