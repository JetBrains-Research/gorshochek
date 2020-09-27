#ifndef INCLUDE_TRANSFORMATIONS_FORTOWHILETRANSFORMATION_H_
#define INCLUDE_TRANSFORMATIONS_FORTOWHILETRANSFORMATION_H_

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
clang::DeclGroupRef, clang::ForStmt, clang::SourceManager, clang::LangOptions,
clang::ContinueStmt, clang::Stmt;
using std::unique_ptr, std::vector, std::string;

/// RecursiveASTVisitor is a set of actions that are done
/// when a certain node of AST is reached
class ForToWhileVisitor : public RecursiveASTVisitor<ForToWhileVisitor> {
 public:
    explicit ForToWhileVisitor(Rewriter * rewriter);
    /**
     * This function is called a certain clang::ForStmt is visited
     */
    bool VisitForStmt(ForStmt * forStmt);
 private:
    Rewriter * rewriter;
    SourceManager & sm;
    LangOptions opt;
    void collectContinues(const Stmt * s, vector<const ContinueStmt *> &continues);  // NOLINT
    void processBody(ForStmt * forStmt);
    void processInit(ForStmt * forStmt);
    void processCond(ForStmt * forStmt);
    void processInc(ForStmt * forStmt);
};

class ForToWhileASTConsumer : public ASTConsumer {
 public:
    explicit ForToWhileASTConsumer(Rewriter * rewriter);
    /**
     * HandleTopLevelDecl handles all the declaration (or definition),
     * e.g. a variable, typedef, function, struct, etc
     * @param DR     Iterating through DeclGroupRef we are getting all the declarations
     *               belongs to current DeclGroup
     * @return       true to continue parsing, or false to abort parsing.
     */
    bool HandleTopLevelDecl(DeclGroupRef DR);
 private:
    ForToWhileVisitor visitor;
};

class ForToWhileTransformation : public ITransformation {
 public:
    explicit ForToWhileTransformation(const float p);
    ~ForToWhileTransformation() = default;
    unique_ptr<ASTConsumer> getConsumer(Rewriter *rewriter);
    static ITransformation * buildFromConfig(const YAML::Node & config);
};

#endif   // INCLUDE_TRANSFORMATIONS_FORTOWHILETRANSFORMATION_H_
