#include "../include/ITransformation.h"
#include "../include/TransformationFrontendAction.h"

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
