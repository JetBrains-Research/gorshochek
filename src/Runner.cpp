#include <string>
#include <vector>
#include <random>
#include <filesystem>
#include <iostream>

#include "../include/Runner.h"
#include "../include/TransformationFrontendAction.h"
#include "../include/TransformationFrontendActionFactory.h"

using clang::tooling::FrontendActionFactory,
clang::tooling::CommonOptionsParser, clang::tooling::ClangTool,
clang::FrontendAction;
using std::size_t, std::vector, std::string, std::ifstream, std::mt19937;
namespace fs = std::filesystem;

const int SEED = 7;

Runner::Runner(const vector<ITransformation *> *transformations,
               size_t n_transformations):
        transformations(transformations),
        n_transformations(n_transformations),
        gen(new mt19937(SEED)) {}

void Runner::createOutputFolders(int num_files, const char * input_files[], string& output_path) {
    fs::path output_dir(output_path);
    if (!fs::exists(output_dir)) {
        fs::create_directory(output_dir);
    }
    // Skipping the first arg which in argv is current program
    for (size_t file_index = 1; file_index < num_files; ++file_index) {
        fs::path current_file_path(input_files[file_index]);
        fs::path transformations_path = fs::path(output_path) / current_file_path.stem();
        if (!fs::exists(transformations_path)) {
            fs::create_directory(transformations_path);
            fs::copy(current_file_path, transformations_path / fs::path("transformation_0.cpp"));
        }
    }
}
void Runner::run(int num_files, const char * files[], const string output_path) {
    createOutputFolders(num_files, files, output_path);
    auto OptionsParser = CommonOptionsParser(num_files, files, TransformationCategory);
    // Constructs a clang tool to run over a list of files.
    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());
    for (size_t i = 0; i < n_transformations; ++i) {
        // Run the Clang Tool, creating a new FrontendAction
        // The way to create new FrontendAction is similar to newFrontendActionFactory function
        Tool.run(std::unique_ptr<FrontendActionFactory>(
                new TransformationFrontendActionFactory(
                        transformations, output_path, gen)).get());
    }
}


