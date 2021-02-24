#include <omp.h>

#include <string>
#include <vector>
#include <map>
#include <random>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "include/Logger.h"
#include "../include/Runner.h"
#include "../include/TransformationFrontendAction.h"
#include "../include/TransformationFrontendActionFactory.h"


using clang::tooling::FrontendActionFactory, clang::tooling::ClangTool,
clang::FrontendAction, clang::tooling::newFrontendActionFactory, clang::tooling::getInsertArgumentAdjuster;
using std::size_t, std::vector, std::string, std::ofstream, std::ios_base, std::to_string,
std::mt19937, std::copy, std::uniform_real_distribution, std::to_string, std::min, std::map,
std::cout, std::endl;

const int SEED = 7;

Runner::Runner(const vector<ITransformation *> *transformations,
               size_t n_transformations, size_t batch_size,
               bool logging_flag):
        transformations(transformations),
        n_transformations(n_transformations),
        batch_size(batch_size),
        logging_flag(logging_flag),
        gen(new mt19937(SEED)) {}

void Runner::createDescriptionFile(const string& file_path,
                                   const string &description) {
    if (fs::is_regular_file(fs::path(file_path))) {
        fs::path description_path = fs::path(file_path).parent_path() / "description.txt";
        ofstream description_stream(description_path, ios_base::app);
        description_stream << description;
    }
}

void Runner::createOutputFolders(const string& input_path,
                                 const string& output_path,
                                 vector<char **> * rewritable_cpaths,
                                 vector<vector<vector<string> *> *> * rewritable_batched_string_paths,
                                 size_t * num_files) const const {
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
    int file_index, batch_index;
    size_t files_left;
    size_t num_batches = ceil(static_cast<float>(*num_files) / batch_size);

    for (int transform_index = 0; transform_index < n_transformations + 1; ++transform_index) {
        if (transform_index != 0) {
            rewritable_cpaths->at(transform_index - 1) = new char * [*num_files];
            rewritable_batched_string_paths->at(transform_index - 1) = new vector<vector<string> *>(num_batches);
            files_left = static_cast<size_t>(*num_files);
            for (batch_index = 0; batch_index < num_batches; ++batch_index) {
                rewritable_batched_string_paths->at(transform_index - 1)
                                               ->at(batch_index) = new vector<string>(min(batch_size, files_left));
                files_left = files_left - batch_size;
            }
        }
        string transformation_name = "transformation_" + to_string(transform_index) + ".cpp";
        file_index = 0;

        for (const auto &src_path : files) {
            batch_index = floor(static_cast<float>(file_index) / batch_size);
            fs::path transformations_path = fs::path(output_path) / src_path.stem();
            fs::create_directory(transformations_path);
            fs::path dst_path = transformations_path / fs::path(transformation_name);
            fs::copy(src_path, dst_path, copy_options);
            if (transform_index != 0) {
                rewritable_batched_string_paths->at(transform_index - 1)
                        ->at(batch_index)->at(file_index % batch_size) = dst_path.string();
                rewritable_cpaths->at(transform_index - 1)[file_index] = new char[dst_path.string().size() + 1];
                strcpy(rewritable_cpaths->at(transform_index - 1)[file_index], dst_path.string().c_str()); // NOLINT
            }
            file_index++;
        }
    }
}

void Runner::createOptionsParser(size_t num_files, vector<char **> * rewritable_cpaths,
            vector<CommonOptionsParser *> * option_parsers) {
    int argc = num_files + 3;
    const char *argv[argc];
    argv[0] = "./gorshochek";
    argv[1] = "-p";
    argv[2] = "build";

    for (size_t transform_index = 0; transform_index < n_transformations; ++transform_index) {
        copy(rewritable_cpaths->at(transform_index),
             rewritable_cpaths->at(transform_index) + num_files,
             argv + 3);
        option_parsers->at(transform_index) = new CommonOptionsParser(argc, argv, TransformationCategory);
    }
}

void Runner::run(const string& input_path, const string& output_path) {
    size_t num_files;
    auto rewritable_cpaths = new vector<char **>(n_transformations);
    auto rewritable_batched_string_paths = new vector<vector<vector<string> *> *>(n_transformations);
    createOutputFolders(input_path, output_path, rewritable_cpaths, rewritable_batched_string_paths, &num_files);

    vector<map<string, string>> descr_per_transform(n_transformations);

    auto num_batches = ceil(static_cast<float>(num_files) / batch_size);
    auto option_parsers = new vector<CommonOptionsParser *>(n_transformations);
    createOptionsParser(num_files, rewritable_cpaths, option_parsers);

    uniform_real_distribution<double> dis(0.0, 1.0);
    size_t transform_index = 0;
    size_t batch_idx = 0;
    ofstream log_stream;

    for (transform_index = 0; transform_index < n_transformations; ++transform_index) {
        auto log_path = fs::path(output_path) / fs::path("log_" + to_string(transform_index + 1) + ".txt");
        if (logging_flag) {
            string init = "initial";
            logTransfromation(&log_path, &init);
        }
        // Run the Clang Tool, creating a new FrontendAction
        // The way to create new FrontendAction is similar to newFrontendActionFactory function
        for (auto transformation : *transformations) {
            cout << "Transformation " << transformation->getName() << endl;
            // Constructs a clang tool to run over a list of files.
            for (batch_idx = 0; batch_idx < num_batches; ++batch_idx) {
                if (dis(*gen) < transformation->getProbability()) {
                    printBatch(rewritable_batched_string_paths->at(transform_index)->at(batch_idx), &batch_idx);
                    for (const auto &batch_file : *rewritable_batched_string_paths->at(transform_index)->at(
                            batch_idx)) {
                        if (descr_per_transform[transform_index].find(batch_file)
                            == descr_per_transform[transform_index].end()) {
                            descr_per_transform[transform_index][batch_file] = "";
                        }
                        descr_per_transform[transform_index][batch_file] +=
                                "\t\t" + transformation->getName() + "\n";
                    }
                    ClangTool Tool(option_parsers->at(transform_index)->getCompilations(),
                                   *rewritable_batched_string_paths->at(transform_index)->at(batch_idx));
                    Tool.appendArgumentsAdjuster(getInsertArgumentAdjuster("-ferror-limit=0"));
                    if (logging_flag) {
                        Tool.run(newFrontendActionFactory<LoggerFrontendAction>().get());
                    }
                    Tool.run(std::unique_ptr<FrontendActionFactory>(
                            new TransformationFrontendActionFactory(transformation)).get());
                }
            }
            if (logging_flag) {
                logTransfromation(&log_path, &transformation->getName());
            }
        }
        if (logging_flag) {
            cout << "Last check " << endl;
            for (batch_idx = 0; batch_idx < num_batches; ++batch_idx) {
                printBatch(rewritable_batched_string_paths->at(transform_index)->at(batch_idx), &batch_idx);
                ClangTool Tool(option_parsers->at(transform_index)->getCompilations(),
                               *rewritable_batched_string_paths->at(transform_index)->at(batch_idx));
                Tool.appendArgumentsAdjuster(getInsertArgumentAdjuster("-ferror-limit=0"));
                Tool.run(newFrontendActionFactory<LoggerFrontendAction>().get());
            }
        }
    }

    map<string, string> descr_per_file;
    for (transform_index = 0; transform_index < n_transformations; ++transform_index) {
        for (const auto &file_descr : descr_per_transform[transform_index]) {
            if (descr_per_file.find(file_descr.first) == descr_per_file.end()) {
                descr_per_file[file_descr.first] = "transformation_" + to_string(transform_index + 1) + "\n";
            }
            descr_per_file[file_descr.first] += file_descr.second;
        }
    }
    for (const auto &file_descr : descr_per_file) {
        createDescriptionFile(file_descr.first, file_descr.second);
    }
}

void Runner::logTransfromation(const fs::path * log_path, const string * name) {
    ofstream log_stream;
    log_stream.open(*log_path, ios_base::app);
    log_stream << " - " << *name << "\n";
    log_stream.close();
}

void Runner::printBatch(const vector<string> * batch, const size_t * batch_idx) {
    cout << "- Batch " << *batch_idx << endl;
    for (const auto& batch_file : *batch) {
        cout << " - - " << batch_file << endl;
    }
    cout << endl << endl;
}
