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
 * Util for extracting the number of transformations from config
 * @param config_path    path to the .yaml config
 * @return               output path
 */
size_t getNumTransformationsFromYaml(const string &config_path);
/**
 * Util for extracting the batch size from config. Our tool
 * supports batch-vise transformations, and batch size
 * defines the number of files that will be processed by ClangTool
 * in one step
 * @param config_path    path to the .yaml config
 * @return               batch size
 */
size_t getBatchSizeFromYaml(const string &config_path);
/**
 * Util for extracting the flag of logging from config.
 * If the param has true value than the number of warnings
 * occurred after applying of each transformetion logged.
 * @param config_path    path to the .yaml config
 * @return               batch size
 */
bool getLoggingFlagFromYaml(const string &config_path);
#endif  // INCLUDE_UTILS_H_
