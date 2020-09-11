#include "../../include/transformations/IdentityTransformation.h"

// ------------ IdentityTransformation ------------

IdentityTransformation::IdentityTransformation(const YAML::Node & config) : ITransformation(config, "identity") {}

unique_ptr<ASTConsumer> IdentityTransformation::getConsumer(Rewriter * rewriter) {
    return llvm::make_unique<ASTConsumer>();
}

ITransformation * IdentityTransformation::buildFromConfig(const YAML::Node & config) {
    return new IdentityTransformation(config);
}

