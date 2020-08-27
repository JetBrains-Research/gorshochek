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
    /**
     * Base class for all transformations, which performs
     * storing data about transformation and creating ASTConsumer instances
     */
 public:
    /**
     *
     * @param p         The probability of certain transformation to be applied
     * @param name      The name of a transformation is used to log applied transformations
     *                  in file description.txt
     */
    explicit ITransformation(float p, string name);
    virtual ~ITransformation();
    /**
     * ASTConsumer is an interface for interacting with AST, this abstraction layer allows
     * to be independent of the AST producer
     * @param rewriter   An instance of class clang::Rewriter which performs all
     *                   transformations on the code
     * @return           Pointer to ASTConsumer
     */
    virtual unique_ptr<ASTConsumer> getConsumer(Rewriter * rewriter) = 0;
    float getProbability() const;
    string const & getName() const;

 private:
    float p;
    string name;
};

#endif  // INCLUDE_ITRANSFORMATION_H_

