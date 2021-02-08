#ifndef INCLUDE_RUNNER_H_
#define INCLUDE_RUNNER_H_

#include <string>
#include <vector>
#include <random>

#include "clang/Tooling/CommonOptionsParser.h"
#include "ITransformation.h"

using clang::tooling::ClangTool, clang::tooling::CommonOptionsParser, llvm::cl::OptionCategory;
using std::string, std::vector, std::mt19937, std::size_t;
namespace fs = std::filesystem;

static OptionCategory TransformationCategory("Transformation");

/// Class for creating ClangTool and run ClangTool on provided files
class Runner {
 public:
    explicit Runner(const vector<ITransformation * > * transformations,
                    size_t n_transformations, size_t batch_size,
                    bool logging_flag);
    void run(const string& input_path, const string& output_path);

 private:
    /**
     * Method for creating folders for transformations and their descriptions
     * @param input_path         Path to dir consisting files that are going to be transformed
     * @param output_path        Path to the output dir where transformed code will be stored
     * @param rewritable_cpaths  Vector of array with c_strings got ClangTool
     * @param num_files          Num files to be transformed
     */
    void createOutputFolders(const string& input_path,
                             const string& output_path,
                             vector<char **> * rewritable_cpaths,
                             vector<vector<vector<string> *> *> * rewritable_string_paths,
                             size_t * num_files) const const;
    static void createDescriptionFile(const string& file_path,
                                      const string &description);
    void createOptionsParser(size_t num_files, vector<char **> * rewritable_cpaths,
                             vector<CommonOptionsParser *> * option_parsers);
    static void logTransfromation(const fs::path log_path, const string name);
    static void printBatch(const vector<string> * batch, const size_t * batch_idx);
    const vector<ITransformation * > * transformations;
    size_t n_transformations;
    size_t batch_size;
    bool logging_flag;
    mt19937 *gen;
};

#endif  // INCLUDE_RUNNER_H_
