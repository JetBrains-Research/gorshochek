#include <string>
#include <vector>
#include <random>
#include <filesystem>
#include <iostream>
#include <fstream>

#include "../include/Runner.h"
#include "../include/TransformationFrontendAction.h"
#include "../include/TransformationFrontendActionFactory.h"

using clang::tooling::FrontendActionFactory,
clang::tooling::CommonOptionsParser, clang::tooling::ClangTool,
clang::FrontendAction;
using std::size_t, std::vector, std::string, std::transform, std::map, std::ofstream, std::ios_base,
std::ifstream, std::mt19937, std::copy, std::back_inserter, std::endl;
namespace fs = std::filesystem;

const int SEED = 7;

Runner::Runner(const vector<ITransformation *> *transformations,
               size_t n_transformations):
        transformations(transformations),
        n_transformations(n_transformations),
        gen(new mt19937(SEED)) {}

map<int, char **> Runner::createOutputFolders(int num_files, char * input_files[], const string& output_path) {
    fs::path output_dir(output_path);
    if (!fs::exists(output_dir)) {
        fs::create_directory(output_dir);
    }
    map<int, vector<string>> rewritable_paths;
    map<int, char **> rewritable_cpaths;
    const auto copy_options = fs::copy_options::overwrite_existing;

    for (int i = 0; i < n_transformations + 1; ++i) {
        if (i != 0) {
            rewritable_cpaths[i] = new char * [num_files];
        }
        string transformation_name = "transformation_" + std::to_string(i) + ".cpp";
        for (size_t file_index = 0; file_index < num_files; ++file_index) {
            fs::path src_path(input_files[file_index]);
            fs::path transformations_path = fs::path(output_path) / src_path.stem();
            if (!fs::exists(transformations_path)) {
                fs::create_directory(transformations_path);
            }
            fs::path dst_path = transformations_path / fs::path(transformation_name);
            fs::copy(src_path, dst_path, copy_options);
            if (i != 0) {
                rewritable_cpaths[i][file_index] = new char[dst_path.string().size() + 1];
                strcpy(rewritable_cpaths[i][file_index], dst_path.string().c_str()); // NOLINT
                fs::path description_path = transformations_path / "description.txt";
                ofstream description(description_path, ios_base::app);
                description << "transformation_" << i << endl;
                for (auto transformation : *transformations) {
                    description << "\t\t" << transformation->getName() << endl;
                }
            }
        }
    }
    return rewritable_cpaths;
}
void Runner::run(int num_files, char ** files, const string& output_path) {
    map<int, char **> rewritable_cpaths = createOutputFolders(num_files, files, output_path);
    int argc = num_files + 3;
    const char * argv[num_files + 3];
    argv[0] = "./gorshochek";
    argv[1] = "-p";
    argv[2] = "build";

    for (size_t i = 0; i < n_transformations; ++i) {
        copy(rewritable_cpaths[i + 1], rewritable_cpaths[i + 1] + num_files, argv + 3);

        auto OptionsParser = CommonOptionsParser(argc, argv,
                                                 TransformationCategory);
        // Run the Clang Tool, creating a new FrontendAction
        // The way to create new FrontendAction is similar to newFrontendActionFactory function
        for (auto transformation : *transformations) {
            // Constructs a clang tool to run over a list of files.
            ClangTool Tool(OptionsParser.getCompilations(),
                           OptionsParser.getSourcePathList());
            Tool.run(std::unique_ptr<FrontendActionFactory>(
                    new TransformationFrontendActionFactory(transformation)).get());
        }
    }
}


