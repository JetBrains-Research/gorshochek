#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

using namespace llvm;
using namespace clang::tooling;

static llvm::cl::OptionCategory TransformationCategory("Basic transformation");

int main(int argc, const char **argv) {
    CommonOptionsParser OptionsParser(argc, argv, TransformationCategory);
    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());
    return Tool.run(newFrontendActionFactory<clang::SyntaxOnlyAction>().get());
}