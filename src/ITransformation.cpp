#include "../include/ITransformation.h"

// ------------ ITransformation ------------

ITransformation::ITransformation(float p) : p(p) {}

ITransformation::~ITransformation() {}

unique_ptr<ASTConsumer> ITransformation::getConsumer(Rewriter *rewriter) {
    return llvm::make_unique<ASTConsumer>();
}

float ITransformation::getProbability() {
    return p;
}
