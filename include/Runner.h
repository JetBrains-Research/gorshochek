#ifndef INCLUDE_RUNNER_H_
#define INCLUDE_RUNNER_H_

#include <string>
#include <vector>
#include <random>

#include "ITransformation.h"

using clang::tooling::ClangTool, llvm::cl::OptionCategory;
using std::string, std::vector, std::mt19937, std::size_t, std::map;

static OptionCategory TransformationCategory("Transformation");

/// Class for creating ClangTool and run ClangTool on provided files
class Runner {
 public:
    explicit Runner(const vector<ITransformation * > * transformations,
                    size_t n_transformations);
    void run(int num_files, char ** files, const string& output_path);
 private:
    /**
     * Method for creating folders for transformations and their descriptions
     * @param num_files     Number of files to be transformed
     * @param input_files   Paths to files to be transformed
     * @param output_path   Path to the output dir where transformed code will be stored
     */
    void createOutputFolders(int num_files, char * input_files[],
                             const string& output_path, vector<char **> * rewritable_cpaths);
    static void createDescriptionFile(int num_files, char ** files,
                                      const string& output_path, const string& description);
    const vector<ITransformation * > * transformations;
    size_t n_transformations;
    mt19937 *gen;
};

#endif  // INCLUDE_RUNNER_H_
