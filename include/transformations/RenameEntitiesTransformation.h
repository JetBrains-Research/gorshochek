#ifndef INCLUDE_TRANSFORMATIONS_RENAMEENTITIESTRANSFORMATION_H_
#define INCLUDE_TRANSFORMATIONS_RENAMEENTITIESTRANSFORMATION_H_

#include <yaml-cpp/yaml.h>

#include <string>
#include <vector>
#include <memory>
#include <map>
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

#include "../ITransformation.h"
#include "./renaming/BaseRenameProcessor.h"

using clang::ASTConsumer, clang::Rewriter, clang::RecursiveASTVisitor, clang::FunctionDecl,
clang::ASTContext, clang::CallExpr, clang::Decl, clang::Stmt, clang::SourceManager;
using std::unique_ptr, std::vector, std::map, std::string, std::mt19937,
std::discrete_distribution;

/// RecursiveASTVisitor is a set of actions that are done
/// when a certain node of AST is reached. RenameEntitiesVisitor in particular
/// visits all entities (e.g functions or variables) and rename them.
/// Currently supported renamings:
///    - random name
///    - hash taken from previous name with a certain prefix
class RenameEntitiesVisitor : public RecursiveASTVisitor<RenameEntitiesVisitor> {
 public:
    explicit RenameEntitiesVisitor(Rewriter * rewriter, const bool rename_func, const bool rename_var,
                                   BaseRenameProcessor * processor);
    /**
     * This function is called a certain clang::CallExpr is visited. Here get all the functions
     * calls/declarations and rename them
     */
    bool VisitCallExpr(CallExpr * call);
    /**
     * This function is called a certain clang::FunctionDecl is visited. Here get all the functions
     * declarations and rename them
     */
    bool VisitFunctionDecl(FunctionDecl * fdecl);
    /**
     * This function is called a certain clang::Stmt is visited. Here get all the variables
     * calls/declarations and rename them
     */
    bool VisitStmt(Stmt * stmt);

 private:
    Rewriter * rewriter;
    SourceManager & sm;

    const bool rename_func = false;
    const bool rename_var = false;

    BaseRenameProcessor * processor;

    map<Decl *, string> decl2name;
    void processVarDecl(Decl * decl, string * name);
};

class RenameEntitiesASTConsumer : public ASTConsumer {
 public:
    explicit RenameEntitiesASTConsumer(Rewriter * rewriter, const bool rename_func, const bool rename_var,
                                       BaseRenameProcessor * processor);
    void HandleTranslationUnit(ASTContext &ctx); // NOLINT.
 private:
    RenameEntitiesVisitor visitor;
    Rewriter * rewriter;
};

class RenameEntitiesTransformation : public ITransformation {
 public:
    explicit RenameEntitiesTransformation(const float p, const bool rename_func,
                                          const bool rename_var,
                                          BaseRenameProcessor * processor);

    ~RenameEntitiesTransformation() = default;
    unique_ptr<ASTConsumer> getConsumer(Rewriter *rewriter);
    static ITransformation * buildFromConfig(const YAML::Node & config);

 private:
    /**
     * Along with probability, the RenameEntitiesTransformation class constructor also accepts
     * entities which are going to renamed to random name
     */
    const bool rename_func = false;
    const bool rename_var = false;

    BaseRenameProcessor * processor;
};

#endif  // INCLUDE_TRANSFORMATIONS_RENAMEENTITIESTRANSFORMATION_H_
