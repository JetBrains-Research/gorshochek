#ifndef INCLUDE_TRANSFORMATIONS_IDENTITYTRANSFORMATION_H_
#define INCLUDE_TRANSFORMATIONS_IDENTITYTRANSFORMATION_H_

#include <yaml-cpp/yaml.h>

#include <string>
#include <memory>

#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/ASTConsumers.h"

#include "../ITransformation.h"

using clang::ASTConsumer;


class IdentityTransformation : public ITransformation {
 public:
    /**
     * Identity transformations does not change code
     */
    explicit IdentityTransformation(const float p);
    ~IdentityTransformation() = default;
    unique_ptr<ASTConsumer> getConsumer(Rewriter *rewriter);
    static ITransformation * buildFromConfig(const YAML::Node & config);
};

#endif  // INCLUDE_TRANSFORMATIONS_IDENTITYTRANSFORMATION_H_
