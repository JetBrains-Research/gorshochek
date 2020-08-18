#ifndef GORSHOCHEK_RUNNER_H
#define GORSHOCHEK_RUNNER_H

#include <iostream>
#include <string>
#include <vector>
#include "clang/Tooling/Tooling.h"

using std::string, std::vector, clang::tooling::ClangTool;

typedef vector<string> TransformationsType;

class Runner {
public:
    void run();
    void applyTransformations(string path, TransformationsType transformation);
private:
    vector<TransformationsType> transformations;
    vector<string> paths;
    void createOutputDir(string path);
    ClangTool createTool(string path);
    void createDescription(string path, TransformationsType transformation);
};

#endif //GORSHOCHEK_RUNNER_H
