#include <filesystem>

#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "llvm/Support/raw_ostream.h"

#include "../include/ITransformation.h"
#include "../include/TransformationFrontendAction.h"

using clang::ASTFrontendAction, clang::ASTConsumer, clang::CompilerInstance,
    clang::StringRef, clang::Rewriter, clang::RecursiveASTVisitor,
    clang::DeclGroupRef, clang::SourceManager, clang::ASTContext,
    clang::ASTMutationListener, clang::ASTDeserializationListener;
using llvm::outs;
using llvm::sys::fs::F_None;
using std::unique_ptr, std::vector, std::move, std::uniform_real_distribution,
std::error_code, std::to_string, std::distance, std::size_t;
namespace fs = std::filesystem;

// ----------- Frontend Actions ------------ //

TransformationFrontendAction::TransformationFrontendAction(
    const vector<ITransformation *> *transformations,
    const string output_path,
    mt19937 *gen
):
        ASTFrontendAction(),
        transformations(transformations),
        output_path(output_path),
        gen(gen) {}

unique_ptr<ASTConsumer> TransformationFrontendAction::CreateASTConsumer(
        CompilerInstance &CI, StringRef file
) {
    rewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    uniform_real_distribution<double> dis(0.0, 1.0);
    vector<unique_ptr<ASTConsumer>> consumers;
    for (auto transformation : *transformations) {
        if (dis(*gen) < transformation->getProbability()) {
            consumers.push_back(transformation->getConsumer(&rewriter));
        }
    }
    // MultiplexConsumer is a kind of ASTConsumer that run multiple consumers provided
    // as vector<unique_ptr<ASTConsumer>>
    return std::make_unique<MultiplexConsumer>(move(consumers));
}

void TransformationFrontendAction::EndSourceFileAction() {
    SourceManager &SM = rewriter.getSourceMgr();
    fs::path current_file_path(this->getCurrentFile().str());
    fs::path transformations_path = fs::path(output_path) / current_file_path.stem();
    size_t cur_transform_index = (size_t)distance(fs::directory_iterator{transformations_path},
                                                  fs::directory_iterator{});
    fs::path cur_transform_path = transformations_path / fs::path("transformation_" + \
                                                                  to_string(cur_transform_index) + \
                                                                  ".cpp");
    // Send rewritten buffer to file "output_path/original_file_name/transformation_X.cpp"
    error_code err_code;
    llvm::raw_fd_ostream out_file(cur_transform_path.string(), err_code, F_None);
    rewriter.getEditBuffer(SM.getMainFileID()).write(out_file);
    out_file.close();
}
