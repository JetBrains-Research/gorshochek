#ifndef INCLUDE_RUNNER_H_
#define INCLUDE_RUNNER_H_

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
};

#endif  //INCLUDE_RUNNER_H_
