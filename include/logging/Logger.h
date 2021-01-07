#ifndef GORSHOCHEK_LOGGER_H
#define GORSHOCHEK_LOGGER_H

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

#include "../ITransformation.h"

using clang::ASTConsumer, clang::ASTContext, clang::ASTFrontendAction, clang::FixItOptions, clang::FixItRewriter;
using clang::CompilerInstance, clang::StringRef;
using std::string;

class LoggingOptions : public FixItOptions {
public:
    LoggingOptions();
    string RewriteFilename(const string &Filename, int &fd) override;
};


class LoggingFrontendAction : public ASTFrontendAction {
    std::unique_ptr<FixItRewriter> rewriter = nullptr;

public:
    LoggingFrontendAction() {}
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(CompilerInstance & CI, StringRef file) override;
    void EndSourceFileAction() override;
};


class FixItASTConsumer : public clang::ASTConsumer {

public:
    // override the constructor in order to pass CI
    explicit FixItASTConsumer();
    void HandleTranslationUnit(ASTContext &ctx) override;
};

#endif //GORSHOCHEK_LOGGER_H
