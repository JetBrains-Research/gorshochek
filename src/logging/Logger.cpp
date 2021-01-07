#include "../include/logging/Logger.h"

std::unique_ptr<ASTConsumer> LoggingFrontendAction::CreateASTConsumer(CompilerInstance & CI, StringRef file) {
    auto fixItOptions = new LoggingOptions();

    rewriter = llvm::make_unique<FixItRewriter>(
            CI.getDiagnostics(),
            CI.getASTContext().getSourceManager(),
            CI.getASTContext().getLangOpts(),
            fixItOptions);

    CI.getDiagnostics().setClient(rewriter.get(), false);

    return llvm::make_unique<FixItASTConsumer>();
}

void LoggingFrontendAction::EndSourceFileAction() {
    assert(rewriter != nullptr);
    rewriter->WriteFixedFiles();
}

LoggingOptions::LoggingOptions() {
    InPlace = false;
    FixWhatYouCan = false;
    FixOnlyWarnings = false;
    Silent = false;
}

string LoggingOptions::RewriteFilename(const string &Filename, int &fd) {
    std::cerr << "------- Bad file " << Filename << "\n";
    fd = -1;
    return Filename;
}

void FixItASTConsumer::HandleTranslationUnit(ASTContext &ctx) {}

FixItASTConsumer::FixItASTConsumer() {}
