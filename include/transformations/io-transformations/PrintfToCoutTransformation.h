#ifndef INCLUDE_TRANSFORMATIONS_IO_TRANSFORMATIONS_PRINTFTOCOUTTRANSFORMATION_H_
#define INCLUDE_TRANSFORMATIONS_IO_TRANSFORMATIONS_PRINTFTOCOUTTRANSFORMATION_H_

#include <yaml-cpp/yaml.h>

#include <string>
#include <vector>
#include <memory>
#include <regex> // NOLINT

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

#include "../../ITransformation.h"

using clang::ASTConsumer, clang::Rewriter, clang::RecursiveASTVisitor, clang::CallExpr,
clang::StringRef, clang::ASTContext, clang::SourceManager, clang::LangOptions,
clang::SourceRange, clang::Expr, clang::UsingDirectiveDecl;
using std::unique_ptr, std::vector, std::string;

/// RecursiveASTVisitor is a set of actions that are done
/// when a certain node of AST is reached
class PrintfToCoutVisitor : public RecursiveASTVisitor<PrintfToCoutVisitor> {
 public:
    explicit PrintfToCoutVisitor(Rewriter * rewriter);
    /**
     * This function is called a certain clang::Stmt is visited
     */
    bool VisitCallExpr(CallExpr *s);
    bool VisitUsingDirectiveDecl(UsingDirectiveDecl *ud);
    void insertHeaderAtBeginning(string headerstr);
    void rewritePrintf();

 private:
    Rewriter * rewriter;
    SourceManager & sm;
    LangOptions opt;
    bool determineIfSimplePrintfCommand(const CallExpr * e);
    bool rewriteSimplePrintfCommand(const CallExpr *e);
    string append_std(string word);
    vector<string> getPrecisionPrefix(const vector<string> &plholdersvec);
    StringRef getAsText(const Expr * e);
    vector<const CallExpr *> printfexpressions;
    vector<const UsingDirectiveDecl *> usedNamespaces;

    string regexPlaceholders = "(%[diufeEgGcs])|%lld|%lu|%ld|%llu|(%\\.[0-9]*f)";
    string floatPrecisionRegex = "(%\\.[0-9][0-9]*f)";
    string floatSplitRegex = "\\.|f";
};

struct IOTransformationException : public std::exception {
    const char * what() const throw();
};

class PrintfToCoutASTConsumer : public ASTConsumer {
 public:
    explicit PrintfToCoutASTConsumer(Rewriter * rewriter);
    /**
     * HandleTopLevelDecl handles all the declaration (or definition),
     * e.g. a variable, typedef, function, struct, etc
     * @param DR     Iterating through DeclGroupRef we are getting all the declarations
     *               belongs to current DeclGroup
     * @return       true to continue parsing, or false to abort parsing.
     */
    void HandleTranslationUnit(ASTContext &ctx); // NOLINT
 private:
    PrintfToCoutVisitor visitor;
};

class PrintfToCoutTransformation : public ITransformation {
 public:
    explicit PrintfToCoutTransformation(const float p);
    ~PrintfToCoutTransformation() = default;
    unique_ptr<ASTConsumer> getConsumer(Rewriter *rewriter);
    static ITransformation * buildFromConfig(const YAML::Node & config);
};

#endif  // INCLUDE_TRANSFORMATIONS_IO_TRANSFORMATIONS_PRINTFTOCOUTTRANSFORMATION_H_
