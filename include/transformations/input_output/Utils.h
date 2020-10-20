#ifndef INCLUDE_TRANSFORMATIONS_INPUT_OUTPUT_UTILS_H_
#define INCLUDE_TRANSFORMATIONS_INPUT_OUTPUT_UTILS_H_

#include <string>
#include <vector>
#include <regex> // NOLINT

using std::vector, std::string;


void split(const string * input, const string * reg, vector<string> * splits);
void getMatches(const string * input, const string * re, vector<string> * matches);

#endif  // INCLUDE_TRANSFORMATIONS_INPUT_OUTPUT_UTILS_H_
