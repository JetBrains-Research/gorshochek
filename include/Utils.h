#ifndef INCLUDE_UTILS_H_
#define INCLUDE_UTILS_H_

#include <string>
#include <vector>

#include "BaseTransformation.h"

using clang::tooling::ClangTool, llvm::cl::OptionCategory;
using std::string, std::vector;

static OptionCategory TransformationCategory("Transformation");

class Runner {
    /* Class for creating ClangTool form .yaml config specified in argv as a first argument
     * and run ClangTool on files specified in rest arguments in argv */
 public:
    Runner(int argc, const char **argv);
    // Run ClangTool on specified files
    void run();

 private:
    int argc;
    const char **argv;
    // Parsing .yaml config to get transformations from it
    static vector<BaseTransformation> getTransformations(const string &config_path);
};

#endif  // INCLUDE_UTILS_H_
