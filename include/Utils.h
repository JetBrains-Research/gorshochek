#ifndef INCLUDE_UTILS_H_
#define INCLUDE_UTILS_H_

#include <string>
#include <vector>

#include "../include/ITransformation.h"

using std::string, std::vector, std::size_t;

/**
 * Util for extracting a vector of transformations form config
 * @param config_path    path to the .yaml config
 * @return               vector of transformations
 */
vector<ITransformation *> *getTransformationsFromYaml(const string &config_path);
/**
 * Util for extracting an output path where the transformed code will be stored
 * @param config_path    path to the .yaml config
 * @return               output path
 */
string getOutputPathFromYaml(const string &config_path);
/**
 * Util for extracting the number of transformations from config
 * @param config_path    path to the .yaml config
 * @return               output path
 */
size_t getNumTransformationsFromYaml(const string &config_path);

#endif  // INCLUDE_UTILS_H_
