#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "llvm/Support/raw_ostream.h"

#include "../include/BaseTransformation.h"
#include "../include/Common.h"


using std::unique_ptr, std::vector, std::move;
using clang::ASTFrontendAction, \
      clang::ASTConsumer, \
      clang::CompilerInstance, \
      clang::StringRef, \
      clang::Rewriter, \
      clang::RecursiveASTVisitor, \
      clang::DeclGroupRef, \
      clang::SourceManager, \
      clang::ASTContext, \
      clang::ASTMutationListener, \
      clang::ASTDeserializationListener;
using llvm::outs;

// ----------- Frontend Actions ------------ //

BaseFrontendAction::BaseFrontendAction(vector<BaseTransformation> transformations, \
                               string output_path) : \
                ASTFrontendAction(), \
                transformations(transformations), \
                output_path(output_path) {}

unique_ptr<ASTConsumer> BaseFrontendAction::CreateASTConsumer(CompilerInstance &CI,
                                                               StringRef file) {
    rewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    // Seed for the random number engine
    random_device rd;
    // Standard mersenne_twister_engine seeded with rd()
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    vector<unique_ptr<ASTConsumer>> consumers;
    for (auto transformation: transformations) {
        if (dis(gen) < transformation.p)
            consumers.push_back(transformation.getConsumer(rewriter));
    }
    return std::make_unique<MultiplexConsumer>(move(consumers));
}

void BaseFrontendAction::EndSourceFileAction() {
    SourceManager &SM = rewriter.getSourceMgr();

    // Now emit the rewritten buffer.
    rewriter.getEditBuffer(SM.getMainFileID()).write(llvm::outs());
}