#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>

#include "include/Logger.h"

using std::ios_base, std::ofstream, std::move;
using llvm::raw_string_ostream;
namespace fs = std::filesystem;

std::unique_ptr<ASTConsumer> LoggingFrontendAction::CreateASTConsumer(CompilerInstance & CI, StringRef file) {
    auto fixItOptions = new LoggingOptions();

    rewriter = std::make_unique<FixItRewriter>(
            CI.getDiagnostics(),
            CI.getASTContext().getSourceManager(),
            CI.getASTContext().getLangOpts(),
            fixItOptions);

    CI.getDiagnostics().setClient(rewriter.get(), false);

    return llvm::make_unique<FixItASTConsumer>(rewriter.get());
}

void LoggingFrontendAction::EndSourceFileAction() {
    auto current_file = this->getCurrentFile().str();
    std::ifstream t(current_file);
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    auto log_path = fs::path(current_file).parent_path().parent_path() / fs::path("log.cpp");
    ofstream description_stream(log_path, ios_base::app);
    description_stream << "//" << current_file << "\t" << rewriter->getNumErrors() <<  "\n\n\n" << str << "\n\n\n";
}

LoggingOptions::LoggingOptions() {
    InPlace = false;
    FixWhatYouCan = false;
    FixOnlyWarnings = false;
    Silent = false;
}

string LoggingOptions::RewriteFilename(const string &Filename, int &fd) {
    auto log_path = fs::path(Filename).parent_path().parent_path() / fs::path("log.cpp");
    ofstream description_stream(log_path, ios_base::app);
    description_stream << "------- Bad file " << Filename << "\n";
    fd = -1;
    return Filename;
}

void FixItASTConsumer::HandleTranslationUnit(ASTContext &ctx) {
}

FixItASTConsumer::FixItASTConsumer(FixItRewriter * rewriter) : rewriter(move(rewriter)) {}
