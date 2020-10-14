#ifndef INCLUDE_TRANSFORMATIONS_INPUT_OUTPUT_PRINTFTOCOUTTRANSFORMATION_H_
#define INCLUDE_TRANSFORMATIONS_INPUT_OUTPUT_PRINTFTOCOUTTRANSFORMATION_H_

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

#include "../../ITransformation.h"

using clang::ASTConsumer, clang::Rewriter, clang::RecursiveASTVisitor, clang::CallExpr,
clang::StringRef, clang::ASTContext, clang::SourceManager, clang::LangOptions,
clang::SourceRange, clang::Expr, clang::UsingDirectiveDecl, clang::SourceLocation;
using std::unique_ptr, std::vector, std::string;

/// RecursiveASTVisitor is a set of actions that are done
/// when a certain node of AST is reached
class PrintfToCoutVisitor : public RecursiveASTVisitor<PrintfToCoutVisitor> {
 public:
    explicit PrintfToCoutVisitor(Rewriter * rewriter);
    /**
     * This function is called a certain clang::CallExpr is visited
     * here we collect all the printf clang::CallExpr
     */
    bool VisitCallExpr(CallExpr * e);
    /**
     * This function is called a certain clang::UsingDirectiveDecl is visited
     * here we collect all used namespaces
     */
    bool VisitUsingDirectiveDecl(UsingDirectiveDecl * ud);
    void insertHeaderAtBeginning(string * headerString);
    void rewritePrintf();

 private:
    Rewriter * rewriter;
    SourceManager & sm;
    LangOptions opt;
    bool determineIfSimplePrintfCommand(const CallExpr * e);
    bool rewriteSimplePrintfCommand(const CallExpr * e);

    vector<string> * getPrecisionPrefix(const vector<string> * specifiers);
    string * parsePrecision(const vector<string> * matches);
    StringRef * getAsText(SourceRange range);
    vector<const CallExpr *> printfExpressions;
    vector<const UsingDirectiveDecl *> usedNamespaces;

    string regexPlaceholders = "(%[diufeEgGcs]|%lld|%lu|%ld|%llu|(%\\.[0-9]*f))";
    string floatPrecisionRegex = "(%\\.[0-9][0-9]*f)";
    string floatSplitRegex = "\\.|f";

    string default_precision = "6";
    string _setprecision = "std::setprecision";
    string _fixed = "std::fixed";
    string _scientific = "std::scientific";
    string _cout = "std::cout";
};

struct MatcherException : public std::exception {
    const char * what() const throw();
};

class PrintfToCoutASTConsumer : public ASTConsumer {
 public:
    explicit PrintfToCoutASTConsumer(Rewriter * rewriter);
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

#endif  // INCLUDE_TRANSFORMATIONS_INPUT_OUTPUT_PRINTFTOCOUTTRANSFORMATION_H_
