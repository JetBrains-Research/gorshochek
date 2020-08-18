#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include <iostream>
#include <vector>
#include <string>

#include "include/Runner.h"
#include "include/Utils.h"

using llvm::cl::OptionCategory;
using std::vector, std::string, std::cerr, std::cout, std::endl;
using clang::tooling::CommonOptionsParser, clang::tooling::newFrontendActionFactory;
using clang::SyntaxOnlyAction;

static OptionCategory TransformationCategory("Basic transformation");

int main(int argc, const char **argv) {
    if (argc != 2)
        cerr << "Not enough positional command line arguments specified" << endl;

    string config_path = argv[1];
    // Parsing config to get original files paths and transformations for them
    vector<TransformationsType> transformations = getTransformations(config_path);
    vector<string> paths = getPaths(config_path);
    // Creating a parser to parse list of files which paths are passed through argc/argv
    CommonOptionsParser OptionsParser(argc, argv, TransformationCategory);
    // Constructs a clang tool to run over a list of files.
    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());
    // Run the Clang Tool, creating a new FrontendAction
    return Tool.run(newFrontendActionFactory<SyntaxOnlyAction>().get());
}