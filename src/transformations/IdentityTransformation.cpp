#include "../../include/transformations/IdentityTransformation.h"

// ------------ IdentityASTConsumer ---------------

IdentityASTConsumer::IdentityASTConsumer() {}

// ------------ IdentityTransformation ------------

IdentityTransformation::IdentityTransformation(float p) : ITransformation(p) {}

IdentityTransformation::~IdentityTransformation() {}

unique_ptr<ASTConsumer> IdentityTransformation::getConsumer(Rewriter * rewriter) {
    return llvm::make_unique<IdentityASTConsumer>();
}

