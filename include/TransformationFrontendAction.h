#ifndef INCLUDE_APPLYTRANSFORMSFRONTENDACTION_H_
#define INCLUDE_APPLYTRANSFORMSFRONTENDACTION_H_

#include "BaseTransformation.h"

#include <vector>
#include <string>
#include <memory>

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

using clang::ASTFrontendAction, clang::ASTConsumer, clang::CompilerInstance,
    clang::StringRef, clang::Rewriter, clang::RecursiveASTVisitor,
    clang::DeclGroupRef, clang::ASTContext, clang::MultiplexConsumer;
using std::unique_ptr, std::vector;

class TransformationFrontendAction : public ASTFrontendAction {
    /* FrontendAction is an interface to create and run ASTConsumer and then save the result.
     * For each source file provided to the tool, a new FrontendAction is created. */
 public:
    TransformationFrontendAction(vector<BaseTransformation> transformations,
                             string output_path);
    unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                              StringRef file) override;
    void EndSourceFileAction() override;

 private:
    Rewriter rewriter;
    vector<BaseTransformation> transformations;
    string output_path;
};

#endif  // INCLUDE_APPLYTRANSFORMSFRONTENDACTION_H_

