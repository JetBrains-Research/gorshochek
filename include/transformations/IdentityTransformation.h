#ifndef INCLUDE_TRANSFORMATIONS_IDENTITYTRANSFORMATION_H_
#define INCLUDE_TRANSFORMATIONS_IDENTITYTRANSFORMATION_H_

#include <string>
#include <memory>

#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/ASTConsumers.h"

#include "../ITransformation.h"

using clang::ASTConsumer;


class IdentityTransformation : public ITransformation {
 public:
    explicit IdentityTransformation(float p);
    ~IdentityTransformation();
    unique_ptr<ASTConsumer> getConsumer(Rewriter *rewriter);
};

#endif  // INCLUDE_TRANSFORMATIONS_IDENTITYTRANSFORMATION_H_
