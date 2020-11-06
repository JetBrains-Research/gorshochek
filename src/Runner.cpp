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
using std::size_t, std::vector, std::string, std::ofstream, std::ios_base,
std::mt19937, std::copy, std::uniform_real_distribution, std::to_string;
namespace fs = std::filesystem;

const int SEED = 7;

Runner::Runner(const vector<ITransformation *> *transformations,
               size_t n_transformations):
        transformations(transformations),
        n_transformations(n_transformations),
        gen(new mt19937(SEED)) {}

void Runner::createDescriptionFile(int num_files, char ** files,
                                   const string& output_path, const string &description) {
    for (size_t file_index = 0; file_index < num_files; ++file_index) {
        fs::path src_path(files[file_index]);
        fs::path transformations_path = fs::path(output_path) / src_path.stem();
        fs::path description_path = transformations_path / "description.txt";
        ofstream description_stream(description_path, ios_base::app);
        description_stream << description;
    }
}

void Runner::createOutputFolders(int num_files,
                                 char * input_files[],
                                 const string& output_path,
                                 vector<char **> * rewritable_cpaths) {
    fs::path output_dir(output_path);
    fs::create_directory(output_dir);

    const auto copy_options = fs::copy_options::overwrite_existing;

    for (int transform_index = 0; transform_index < n_transformations + 1; ++transform_index) {
        if (transform_index != 0) {
            rewritable_cpaths->at(transform_index - 1) = new char * [num_files];
        }
        string transformation_name = "transformation_" + std::to_string(transform_index) + ".cpp";
        for (size_t file_index = 0; file_index < num_files; ++file_index) {
            fs::path src_path(input_files[file_index]);
            fs::path transformations_path = fs::path(output_path) / src_path.stem();
            fs::create_directory(transformations_path);
            fs::path dst_path = transformations_path / fs::path(transformation_name);
            fs::copy(src_path, dst_path, copy_options);
            if (transform_index != 0) {
                rewritable_cpaths->at(transform_index - 1)[file_index] = new char[dst_path.string().size() + 1];
                strcpy(rewritable_cpaths->at(transform_index - 1)[file_index], dst_path.string().c_str()); // NOLINT
            }
        }
    }
}

void Runner::run(int num_files, char ** files, const string& output_path) {
    auto rewritable_cpaths = new vector<char **>(n_transformations);
    createOutputFolders(num_files, files, output_path, rewritable_cpaths);

    int argc = num_files + 3;
    const char * argv[num_files + 3];
    argv[0] = "./gorshochek";
    argv[1] = "-p";
    argv[2] = "build";

    string description;
    uniform_real_distribution<double> dis(0.0, 1.0);
    for (size_t transform_index = 0; transform_index < n_transformations; ++transform_index) {
        copy(rewritable_cpaths->at(transform_index), rewritable_cpaths->at(transform_index) + num_files, argv + 3);

        description += "transformation_" + to_string(transform_index) + "\n";
        auto OptionsParser = CommonOptionsParser(argc, argv,
                                                 TransformationCategory);
        // Run the Clang Tool, creating a new FrontendAction
        // The way to create new FrontendAction is similar to newFrontendActionFactory function
        for (auto transformation : *transformations) {
            // Constructs a clang tool to run over a list of files.
            if (dis(*gen) < transformation->getProbability()) {
                ClangTool Tool(OptionsParser.getCompilations(),
                               OptionsParser.getSourcePathList());
                Tool.run(std::unique_ptr<FrontendActionFactory>(
                        new TransformationFrontendActionFactory(transformation)).get());
                description += "\t\t" + transformation->getName() + "\n";
            }
        }
    }
    createDescriptionFile(num_files, files, output_path, description);
}


