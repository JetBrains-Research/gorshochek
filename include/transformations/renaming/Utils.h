#ifndef INCLUDE_TRANSFORMATIONS_RENAMING_UTILS_H_
#define INCLUDE_TRANSFORMATIONS_RENAMING_UTILS_H_

#include <random>

using std::discrete_distribution;

discrete_distribution<int> createUniformIntGenerator(int num_elements);

#endif  // INCLUDE_TRANSFORMATIONS_RENAMING_UTILS_H_
