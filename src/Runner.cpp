#include <string>
#include <vector>
#include <random>

#include "../include/Runner.h"
#include "../include/TransformationFrontendAction.h"
#include "../include/TransformationFrontendActionFactory.h"

using clang::tooling::FrontendActionFactory,
clang::tooling::CommonOptionsParser, clang::tooling::ClangTool,
clang::FrontendAction;
using std::size_t, std::vector, std::string, std::ifstream, std::mt19937;

const int SEED = 7;

Runner::Runner(const vector<ITransformation *> *transformations):
        transformations(transformations), gen(new mt19937(SEED)) {}

void Runner::run(int num_files, const char * files[], string output_path) {
    auto OptionsParser = CommonOptionsParser(num_files, files, TransformationCategory);
    // Constructs a clang tool to run over a list of files.
    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());
    // Run the Clang Tool, creating a new FrontendAction
    // The way to create new FrontendAction is similar to newFrontendActionFactory function
    Tool.run(std::unique_ptr<FrontendActionFactory>(
                new TransformationFrontendActionFactory(
                    transformations, move(output_path), gen)).get());
}


