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
using std::unique_ptr, std::string;


class ITransformation {
    /* Class for storing data about transformation and creating ASTConsumer instances */
 public:
    explicit ITransformation(float p, string name);
    virtual ~ITransformation();
    virtual unique_ptr<ASTConsumer> getConsumer(Rewriter * rewriter) = 0;
    float getProbability() const;
    string const & getName() const;
 private:
    float p;
    string name;
};

#endif  // INCLUDE_ITRANSFORMATION_H_

