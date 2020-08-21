#ifndef INCLUDE_RUNNER_H_
#define INCLUDE_RUNNER_H_

#include <string>
#include <vector>
#include <random>

#include "ITransformation.h"

using clang::tooling::ClangTool, llvm::cl::OptionCategory;
using std::string, std::vector, std::mt19937;

static OptionCategory TransformationCategory("Transformation");

class Runner {
    /* Class for creating ClangTool and run ClangTool on files */
 public:
    explicit Runner(vector<ITransformation *> transformations);
    // Run ClangTool on specified files
    void run(int num_files, const char ** files, string output_path);
 private:
    vector<ITransformation *> transformations;
    mt19937 gen;
};

#endif  // INCLUDE_RUNNER_H_
