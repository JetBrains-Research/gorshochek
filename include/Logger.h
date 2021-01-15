#ifndef INCLUDE_LOGGER_H_
#define INCLUDE_LOGGER_H_

#include <yaml-cpp/yaml.h>

#include <string>
#include <memory>
#include <iostream>

#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Frontend/FixItRewriter.h"
#include "clang/Tooling/Tooling.h"

#include "ITransformation.h"

using clang::ASTConsumer, clang::ASTContext, clang::ASTFrontendAction, clang::FixItOptions, clang::FixItRewriter;
using clang::CompilerInstance, clang::StringRef;
using std::string;

class LoggerOptions : public FixItOptions {
 public:
    LoggerOptions();
    string RewriteFilename(const string &Filename, int &fd) override;
};


class LoggerFrontendAction : public ASTFrontendAction {
    std::unique_ptr<FixItRewriter> rewriter = nullptr;

 public:
    LoggerFrontendAction() {}
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance & CI, StringRef file) override;
    void EndSourceFileAction() override;
};


class FixItASTConsumer : public clang::ASTConsumer {
 public:
    explicit FixItASTConsumer(FixItRewriter * rewriter);
    void HandleTranslationUnit(ASTContext &ctx) override;
 private:
    FixItRewriter * rewriter;
};

#endif  // INCLUDE_LOGGER_H_
