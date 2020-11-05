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

TransformationFrontendAction::TransformationFrontendAction(ITransformation * transformation):
        ASTFrontendAction(),
        transformation(transformation) {}

unique_ptr<ASTConsumer> TransformationFrontendAction::CreateASTConsumer(
        CompilerInstance &CI, StringRef file
) {
    rewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    vector<unique_ptr<ASTConsumer>> consumers;
    consumers.push_back(transformation->getConsumer(&rewriter));
    // MultiplexConsumer is a kind of ASTConsumer that run multiple consumers provided
    // as vector<unique_ptr<ASTConsumer>>
    return std::make_unique<MultiplexConsumer>(move(consumers));
}