#ifndef GORSHOCHEK_UTILS_H
#define GORSHOCHEK_UTILS_H

#include <string>
#include <vector>

#include "BaseTransformation.h"

using std::string, std::vector;
using clang::tooling::ClangTool, \
      llvm::cl::OptionCategory;

static OptionCategory TransformationCategory("Transformation");

class Runner{
public:
    Runner(int argc, const char ** argv);
    void run();
private:
    int argc;
    const char ** argv;
    vector<BaseTransformation> getTransformations(const string& config_path);
};

#endif //GORSHOCHEK_UTILS_H