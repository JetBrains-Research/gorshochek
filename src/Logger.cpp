#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>

#include "include/Logger.h"

using std::ios_base, std::ofstream, std::move;
using llvm::raw_string_ostream;
namespace fs = std::filesystem;

std::unique_ptr<ASTConsumer> LoggerFrontendAction::CreateASTConsumer(CompilerInstance & CI, StringRef file) {
    auto fixItOptions = new LoggerOptions();

    rewriter = std::make_unique<FixItRewriter>(
            CI.getDiagnostics(),
            CI.getASTContext().getSourceManager(),
            CI.getASTContext().getLangOpts(),
            fixItOptions);

    CI.getDiagnostics().setClient(rewriter.get(), false);

    return llvm::make_unique<FixItASTConsumer>(rewriter.get());
}

void LoggerFrontendAction::EndSourceFileAction() {
    auto current_file = this->getCurrentFile().str();
    auto log_path = fs::path(current_file).parent_path().parent_path() / fs::path("log.txt");
    ofstream log_stream(log_path, ios_base::app);
    log_stream << current_file << "\t" << rewriter->getNumErrors() <<  "\n";
    log_stream.close();
}

LoggerOptions::LoggerOptions() {
    InPlace = false;
    FixWhatYouCan = false;
    FixOnlyWarnings = false;
    Silent = false;
}

string LoggerOptions::RewriteFilename(const string &Filename, int &fd) {
    fd = -1;
    return Filename;
}

void FixItASTConsumer::HandleTranslationUnit(ASTContext &ctx) {
}

FixItASTConsumer::FixItASTConsumer(FixItRewriter * rewriter) : rewriter(move(rewriter)) {}
