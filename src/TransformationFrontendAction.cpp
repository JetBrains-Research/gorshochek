#include <filesystem>
#include <fstream>
#include <iostream>

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
std::error_code, std::to_string, std::count_if, std::size_t, std::ofstream,
std::cerr, std::endl;
using std::ios_base;
namespace fs = std::filesystem;

// ----------- Frontend Actions ------------ //

TransformationFrontendAction::TransformationFrontendAction(
    const vector<ITransformation *> *transformations,
    string const & output_path,
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

    fs::path transformations_path = getTransformationsPath();
    auto di = fs::directory_iterator{transformations_path};
    size_t cur_transform_index = (size_t)count_if(fs::begin(di), fs::end(di), isFileCpp);
    fs::path description_path = transformations_path / "description.txt";
    ofstream description(description_path, ios_base::app);
    description << "transformation_" << cur_transform_index << endl;
    for (auto transformation : *transformations) {
        if (dis(*gen) < transformation->getProbability()) {
            consumers.push_back(transformation->getConsumer(&rewriter));
            description << "\t\t" << transformation->getName() << endl;
        }
    }

    description.close();
    // MultiplexConsumer is a kind of ASTConsumer that run multiple consumers provided
    // as vector<unique_ptr<ASTConsumer>>
    return std::make_unique<MultiplexConsumer>(move(consumers));
}

fs::path TransformationFrontendAction::getTransformationsPath() {
    fs::path current_file_path(this->getCurrentFile().str());
    fs::path current_path = fs::current_path();
    fs::path path_to_gorshochek;
    if (current_path.filename().compare("build") == 0) {
        path_to_gorshochek = fs::path("..");
    } else if (current_path.filename().compare("yaml-cpp-build") == 0) {
        path_to_gorshochek = fs::path("..") / fs::path("..") / fs::path("..");
        std::cout << "yes";
    } else {
        cerr << "Unexpected current directory" << current_path.string() << endl;
    }
    return path_to_gorshochek / fs::path(output_path) / current_file_path.stem();
}


bool TransformationFrontendAction::isFileCpp(fs::path const &path) {
    return path.extension() == ".cpp";
}


void TransformationFrontendAction::EndSourceFileAction() {
    SourceManager &SM = rewriter.getSourceMgr();
    fs::path transformations_path = getTransformationsPath();
    auto di = fs::directory_iterator{transformations_path};
    size_t cur_transform_index = (size_t)count_if(fs::begin(di), fs::end(di), isFileCpp);
    fs::path cur_transform_path = transformations_path / fs::path("transformation_" + \
                                                                  to_string(cur_transform_index) + \
                                                                  ".cpp");
    // Send rewritten buffer to file "output_path/original_file_name/transformation_X.cpp"
    error_code err_code;
    llvm::raw_fd_ostream out_file(cur_transform_path.string(), err_code, F_None);
    rewriter.getEditBuffer(SM.getMainFileID()).write(out_file);
    out_file.close();
}
