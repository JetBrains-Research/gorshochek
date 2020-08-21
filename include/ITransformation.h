#ifndef INCLUDE_ITRANSFORMATION_H_
#define INCLUDE_ITRANSFORMATION_H_

#include <string>
#include <memory>

#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"


using clang::ASTConsumer, clang::Rewriter;
using std::unique_ptr;


class ITransformation {
    /* Class for storing data about transformation and creating ASTConsumer instances */
 public:
    explicit ITransformation(float p);
    virtual ~ITransformation();
    virtual unique_ptr<ASTConsumer> getConsumer(Rewriter * rewriter);
    float getProbability();
 private:
    float p;
};

#endif  // INCLUDE_ITRANSFORMATION_H_

