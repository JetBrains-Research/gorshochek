#ifndef INCLUDE_RUNNER_H_
#define INCLUDE_RUNNER_H_

#include <string>
#include <vector>
#include <random>

#include "clang/Tooling/CommonOptionsParser.h"
#include "ITransformation.h"

using clang::tooling::ClangTool, clang::tooling::CommonOptionsParser, llvm::cl::OptionCategory;
using std::string, std::vector, std::mt19937, std::size_t;

static OptionCategory TransformationCategory("Transformation");

/// Class for creating ClangTool and run ClangTool on provided files
class Runner {
 public:
    explicit Runner(const vector<ITransformation * > * transformations,
                    size_t n_transformations);
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
                             int * num_files);
    static void createDescriptionFile(const string& input_path,
                                      const string& output_path,
                                      const string &description);
    void createOptionsParser(int num_files, vector<char **> * rewritable_cpaths,
                             vector<CommonOptionsParser *> * option_parsers);
    const vector<ITransformation * > * transformations;
    size_t n_transformations;
    mt19937 *gen;
};

#endif  // INCLUDE_RUNNER_H_
