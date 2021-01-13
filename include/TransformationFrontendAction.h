#ifndef INCLUDE_TRANSFORMATIONFRONTENDACTION_H_
#define INCLUDE_TRANSFORMATIONFRONTENDACTION_H_

#include "ITransformation.h"

#include <vector>
#include <memory>

#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/MultiplexConsumer.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"

using clang::ASTFrontendAction, clang::ASTConsumer, clang::CompilerInstance,
    clang::StringRef, clang::Rewriter, clang::ASTContext, clang::MultiplexConsumer;
using std::unique_ptr, std::vector;

/// FrontendAction is an interface to create and run ASTConsumer and then save the result.
/// For each source file provided to the tool, a new FrontendAction is created.
class TransformationFrontendAction : public ASTFrontendAction {
 public:
    explicit TransformationFrontendAction(ITransformation * transformation);
    unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                              StringRef file) override;

 private:
    Rewriter rewriter;
    ITransformation * transformation;
};

#endif  // INCLUDE_TRANSFORMATIONFRONTENDACTION_H_

