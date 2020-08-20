#ifndef INCLUDE_UTILS_H_
#define INCLUDE_UTILS_H_

#include <string>

#include "../include/BaseTransformation.h"

vector<BaseTransformation> getTransformationsFromYaml(const string &config_path);

#endif  // INCLUDE_UTILS_H_
