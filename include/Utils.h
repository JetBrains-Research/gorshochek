#ifndef GORSHOCHEK_UTILS_H
#define GORSHOCHEK_UTILS_H

#include "Runner.h"
#include <string>
#include <vector>

using std::string, std::vector;

vector<TransformationsType> getTransformations(string config_path);
vector<string> getPaths(string config_path);

#endif //GORSHOCHEK_UTILS_H