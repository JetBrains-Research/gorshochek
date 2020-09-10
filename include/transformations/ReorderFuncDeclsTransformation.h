#ifndef GORSHOCHEK_REORDERFUNCDECLSTRANSFORMATION_H
#define GORSHOCHEK_REORDERFUNCDECLSTRANSFORMATION_H


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

using clang::ASTConsumer, clang::Rewriter, clang::RecursiveASTVisitor,
clang::ASTContext, clang::CallExpr, clang::SourceLocation, clang::FunctionDecl;
using std::unique_ptr, std::vector, std::map, std::string, std::mt19937,
std::discrete_distribution;

/// RecursiveASTVisitor is a set of actions that are done
/// when a certain node of AST is reached. ReorderFuncDeclsVisitor in particular
/// visits all functions declarations unused and reorder them
class ReorderFuncDeclsVisitor : public RecursiveASTVisitor<ReorderFuncDeclsVisitor> {
public:
    explicit ReorderFuncDeclsVisitor(Rewriter * rewriter, mt19937 * gen, const bool test = false);
    /**
     * This function is called a certain clang::CallExpr is visited. Here get all the functions
     * calls and find unused ones
     */
    bool VisitCallExpr(CallExpr * call);
    /**
     * This function is called after visiting all the clang::CallExpr, it reorder functions and place
     * their definition after main function
     */
    void rewriteFunctions();

private:
    Rewriter * rewriter;

    mt19937 * gen;

    const bool test = false;

    vector<FunctionDecl *> funcdecls;
    FunctionDecl * mainDecl;

    bool isFuncDeclProcessed(FunctionDecl * decl);
    static bool compare(FunctionDecl * a, FunctionDecl * b);
};

class ReorderFuncDeclsASTConsumer : public ASTConsumer {
public:
    explicit ReorderFuncDeclsASTConsumer(Rewriter * rewriter, mt19937 * gen, const bool test);
    void HandleTranslationUnit(ASTContext &ctx); // NOLINT.
private:
    ReorderFuncDeclsVisitor visitor;
};

class ReorderFuncDeclsTransformation : public ITransformation {
public:
    explicit ReorderFuncDeclsTransformation(float p, const int seed, const bool test);
    ~ReorderFuncDeclsTransformation();
    unique_ptr<ASTConsumer> getConsumer(Rewriter *rewriter);

private:
    mt19937 * gen;

    const bool test = false;
};

#endif //GORSHOCHEK_REORDERFUNCDECLSTRANSFORMATION_H
