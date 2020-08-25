#ifndef INCLUDE_UTILS_H_
#define INCLUDE_UTILS_H_

#include <string>
#include <vector>

#include "../include/ITransformation.h"

using std::string, std::vector, std::size_t;

vector<ITransformation *> *getTransformationsFromYaml(const string &config_path);
string getOutputPathFromYaml(const string &config_path);
size_t getNumTransformationsFromYaml(const string &config_path);

#endif  // INCLUDE_UTILS_H_
