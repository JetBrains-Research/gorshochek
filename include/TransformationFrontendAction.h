#ifndef INCLUDE_TRANSFORMATIONFRONTENDACTION_H_
#define INCLUDE_TRANSFORMATIONFRONTENDACTION_H_

#include "ITransformation.h"

#include <vector>
#include <string>
#include <memory>
#include <random>
#include <filesystem>

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
using std::unique_ptr, std::vector, std::string, std::vector, std::mt19937;
namespace fs = std::filesystem;

/// FrontendAction is an interface to create and run ASTConsumer and then save the result.
/// For each source file provided to the tool, a new FrontendAction is created.
class TransformationFrontendAction : public ASTFrontendAction {
 public:
    TransformationFrontendAction(const vector<ITransformation *> *transformations,
                                 string const & output_path,
                                 mt19937 *gen);
    unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                              StringRef file) override;
    /**
     * At the end of each file EndSourceFileAction is called, inside this method we
     * dump the transformed code on the disk
     */
    void EndSourceFileAction() override;

 private:
    Rewriter rewriter;
    const vector<ITransformation *> *transformations;
    string const & output_path;
    mt19937 *gen;
    fs::path getTransformationsPath();
    static bool isFileCpp(fs::path const &path);
};

#endif  // INCLUDE_TRANSFORMATIONFRONTENDACTION_H_

