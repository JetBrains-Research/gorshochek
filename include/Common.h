#ifndef GORSHOCHEK_COMMON_H
#define GORSHOCHEK_COMMON_H

#include <iostream>
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/MultiplexConsumer.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"

#include "BaseTransformation.h"

using std::unique_ptr, std::vector;
using clang::ASTFrontendAction, \
      clang::ASTConsumer, \
      clang::CompilerInstance, \
      clang::StringRef, \
      clang::Rewriter, \
      clang::RecursiveASTVisitor, \
      clang::DeclGroupRef, \
      clang::ASTContext, \
      clang::MultiplexConsumer;

// For each source file provided to the tool, a new FrontendAction is created.
class BaseFrontendAction : public ASTFrontendAction {
public:
    BaseFrontendAction(vector<BaseTransformation> transformations, string output_path);
    unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                              StringRef file) override;
    void EndSourceFileAction() override;
private:
    Rewriter rewriter;
    vector<BaseTransformation> transformations;
    string output_path;
};

class AggregateASTConsumer : public MultiplexConsumer {
public:
    explicit AggregateASTConsumer(vector<BaseTransformation> transformations, Rewriter rewriter);
    bool HandleTopLevelDecl(DeclGroupRef DR) override;
private:
    vector<BaseTransformation> transformations;
    Rewriter rewriter;
};


#endif //GORSHOCHEK_COMMON_H
