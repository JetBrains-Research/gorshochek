#include "../../include/transformations/IdentityTransformation.h"

// ------------ IdentityTransformation ------------

IdentityTransformation::IdentityTransformation(const float p) : ITransformation(p, "identity") {}

unique_ptr<ASTConsumer> IdentityTransformation::getConsumer(Rewriter * rewriter) {
    return std::make_unique<ASTConsumer>();
}

ITransformation * IdentityTransformation::buildFromConfig(const YAML::Node & config) {
    const auto p = config["p"].as<float>();
    return new IdentityTransformation(p);
}

