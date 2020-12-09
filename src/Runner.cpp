#include <omp.h>

#include <string>
#include <vector>
#include <random>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

#include "../include/Runner.h"
#include "../include/TransformationFrontendAction.h"
#include "../include/TransformationFrontendActionFactory.h"

using clang::tooling::FrontendActionFactory, clang::tooling::ClangTool,
clang::FrontendAction;
using std::size_t, std::vector, std::string, std::ofstream, std::ios_base,
std::mt19937, std::copy, std::uniform_real_distribution, std::to_string, std::min;
namespace fs = std::filesystem;

const int SEED = 7;

Runner::Runner(const vector<ITransformation *> *transformations,
               size_t n_transformations, size_t batch_size):
        transformations(transformations),
        n_transformations(n_transformations),
        batch_size(batch_size),
        gen(new mt19937(SEED)) {}

void Runner::createDescriptionFile(const string& input_path,
                                   const string& output_path,
                                   const string &description) {
    if (fs::is_regular_file(fs::path(input_path))) {
        fs::path transformations_path = fs::path(output_path) / fs::path(input_path).stem();
        fs::path description_path = transformations_path / "description.txt";
        ofstream description_stream(description_path, ios_base::app);
        description_stream << description;
    } else {
        for (const auto &src_path : fs::recursive_directory_iterator(input_path)) {
            if (fs::is_regular_file(src_path)) {
                fs::path transformations_path = fs::path(output_path) / src_path.path().stem();
                fs::path description_path = transformations_path / "description.txt";
                ofstream description_stream(description_path, ios_base::app);
                description_stream << description;
            }
        }
    }
}

void Runner::createOutputFolders(const string& input_path,
                                 const string& output_path,
                                 vector<char **> * rewritable_cpaths,
                                 size_t * num_files) {
    fs::path output_dir(output_path);
    fs::create_directory(output_dir);

    const auto copy_options = fs::copy_options::overwrite_existing;
    vector<fs::path> files;
    auto fs_input_path = fs::path(input_path);

    if (fs::is_regular_file(fs_input_path)) {
        files.push_back(fs_input_path);
    } else {
        for (const auto &elem : fs::recursive_directory_iterator(fs_input_path)) {
            if (fs::is_regular_file(elem)) {
                files.push_back(elem.path());
            }
        }
    }

    *num_files = files.size();
    int file_index;

    for (int transform_index = 0; transform_index < n_transformations + 1; ++transform_index) {
        if (transform_index != 0) {
            rewritable_cpaths->at(transform_index - 1) = new char * [*num_files];
        }
        string transformation_name = "transformation_" + std::to_string(transform_index) + ".cpp";
        file_index = 0;
        for (const auto& src_path : files) {
            fs::path transformations_path = fs::path(output_path) / src_path.stem();
            fs::create_directory(transformations_path);
            fs::path dst_path = transformations_path / fs::path(transformation_name);
            fs::copy(src_path, dst_path, copy_options);
            if (transform_index != 0) {
                rewritable_cpaths->at(transform_index - 1)[file_index] = new char[dst_path.string().size() + 1];
                strcpy(rewritable_cpaths->at(transform_index - 1)[file_index], dst_path.string().c_str()); // NOLINT
            }
            file_index++;
        }
    }
}

void Runner::createOptionsParser(
        size_t num_files, vector<char **> * rewritable_cpaths,
        vector<vector<CommonOptionsParser *> *> * option_parsers) {
    size_t num_batches = ceil(static_cast<float>(num_files) / batch_size);

    int argc = batch_size + 3;
    const char *argv[argc];
    argv[0] = "./gorshochek";
    argv[1] = "-p";
    argv[2] = "build";

    int last_batch_argc = num_files - (num_batches - 1) * batch_size + 3;
    const char * last_batch_argv[last_batch_argc];
    last_batch_argv[0] = "./gorshochek";
    last_batch_argv[1] = "-p";
    last_batch_argv[2] = "build";

    for (auto transform_index = 0; transform_index < n_transformations; ++transform_index) {
        option_parsers->at(transform_index) = new vector<CommonOptionsParser *>(num_batches);
        for (auto batch_index = 0; batch_index < num_batches; ++batch_index) {
            if ((batch_index + 1) * batch_size < num_files) {
                copy(rewritable_cpaths->at(transform_index) + batch_index * batch_size,
                     rewritable_cpaths->at(transform_index) + (batch_index + 1) * batch_size,
                     argv + 3);
                option_parsers->at(transform_index)
                        ->at(batch_index) = new CommonOptionsParser(argc, argv, TransformationCategory);
            } else {
                copy(rewritable_cpaths->at(transform_index) + batch_index * batch_size,
                     rewritable_cpaths->at(transform_index) + num_files,
                     last_batch_argv + 3);
                option_parsers->at(transform_index)
                              ->at(batch_index) = new CommonOptionsParser(
                                      last_batch_argc, last_batch_argv, TransformationCategory);
            }
        }
    }
}

void Runner::run(const string& input_path, const string& output_path) {
    vector<string> descr_per_transform(n_transformations);
    size_t num_files;
    auto rewritable_cpaths = new vector<char **>(n_transformations);
    createOutputFolders(input_path, output_path, rewritable_cpaths, &num_files);

    auto num_batches = ceil(static_cast<float>(num_files) / batch_size);
    auto option_parsers = new vector<vector<CommonOptionsParser *> *>(n_transformations);
    createOptionsParser(num_files, rewritable_cpaths, option_parsers);

    uniform_real_distribution<double> dis(0.0, 1.0);
    size_t transform_index = 0;
    #pragma omp parallel \
        private(transform_index) \
        shared(num_files, rewritable_cpaths, option_parsers, descr_per_transform)
    {  // NOLINT
        #pragma omp for
        for (transform_index = 0; transform_index < n_transformations; ++transform_index) {
            // Run the Clang Tool, creating a new FrontendAction
            // The way to create new FrontendAction is similar to newFrontendActionFactory function
            for (auto transformation : *transformations) {
                std::cout << "Transformation " << transformation->getName() << "\n";
                // Constructs a clang tool to run over a list of files.
                for (auto batch_idx = 0; batch_idx < num_batches; ++batch_idx) {
                    std::cout << "- Batch " << batch_idx << "\n";
                    if (dis(*gen) < transformation->getProbability()) {
                        ClangTool Tool(option_parsers->at(transform_index)->at(batch_idx)->getCompilations(),
                                       option_parsers->at(transform_index)->at(batch_idx)->getSourcePathList());
                        Tool.run(std::unique_ptr<FrontendActionFactory>(
                                new TransformationFrontendActionFactory(transformation)).get());
                        descr_per_transform[transform_index] += "\t\t" + transformation->getName() + "\n";
                    }
                }
            }
        }
    }

    string description;
    for (const auto & descr : descr_per_transform) {
        description += descr;
    }

    createDescriptionFile(input_path, output_path, description);
}


