#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

using llvm::cl::OptionCategory;
using clang::tooling::CommonOptionsParser, \
      clang::tooling::ClangTool, \
      clang::tooling::newFrontendActionFactory;
using clang::SyntaxOnlyAction;

static OptionCategory TransformationCategory("Basic transformation");

int main(int argc, const char **argv) {
    // Creating a parser to parse list of files
    CommonOptionsParser OptionsParser(argc, argv, TransformationCategory);
    // Constructs a clang tool to run over a list of files.
    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());
    // Run the Clang Tool, creating a new FrontendAction
    return Tool.run(newFrontendActionFactory<SyntaxOnlyAction>().get());
}
