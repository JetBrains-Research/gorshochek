#ifndef INCLUDE_TRANSFORMATIONS_INPUT_OUTPUT_UTILS_H_
#define INCLUDE_TRANSFORMATIONS_INPUT_OUTPUT_UTILS_H_

#include <string>
#include <vector>
#include <regex> // NOLINT

using std::vector, std::string;


vector<string> split(const string * input, const string * reg);
vector<string> getMatches(const string * input, const string * re);

#endif  // INCLUDE_TRANSFORMATIONS_INPUT_OUTPUT_UTILS_H_
