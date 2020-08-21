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
using std::unique_ptr, std::vector, std::move, std::uniform_real_distribution;

// ----------- Frontend Actions ------------ //

TransformationFrontendAction::TransformationFrontendAction(
    vector<ITransformation *> transformations, string output_path, mt19937 gen
):
        ASTFrontendAction(),
        transformations(move(transformations)),
        output_path(move(output_path)),
        gen(gen) {}

unique_ptr<ASTConsumer> TransformationFrontendAction::CreateASTConsumer(
        CompilerInstance &CI, StringRef file
) {
    rewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    uniform_real_distribution<double> dis(0.0, 1.0);
    vector<unique_ptr<ASTConsumer>> consumers;
    for (auto transformation : transformations) {
        if (dis(gen) < transformation->getProbability())
            consumers.push_back(transformation->getConsumer(&rewriter));
    }
    // MultiplexConsumer is a kind of ASTConsumer that run multiple consumers provided
    // as vector<unique_ptr<ASTConsumer>>
    return std::make_unique<MultiplexConsumer>(move(consumers));
}

void TransformationFrontendAction::EndSourceFileAction() {
    SourceManager &SM = rewriter.getSourceMgr();
    // Send rewritten buffer to std::out.
    // TODO(write to file): change llvm::outs() to file specified by output_path
    rewriter.getEditBuffer(SM.getMainFileID()).write(llvm::outs());
}
