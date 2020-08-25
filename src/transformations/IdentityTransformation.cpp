#include "../../include/transformations/IdentityTransformation.h"

// ------------ IdentityTransformation ------------

IdentityTransformation::IdentityTransformation(float p) : ITransformation(p) {}

char * IdentityTransformation::ITransformation::name = "identity";

IdentityTransformation::~IdentityTransformation() {}

unique_ptr<ASTConsumer> IdentityTransformation::getConsumer(Rewriter * rewriter) {
    return llvm::make_unique<ASTConsumer>();
}

