#ifndef INCLUDE_TRANSFORMATIONS_RENAMING_DISTRIBUTIONGENERATOR_H_
#define INCLUDE_TRANSFORMATIONS_RENAMING_DISTRIBUTIONGENERATOR_H_

#include <random>

using std::discrete_distribution;

static class DistributionGenerator {
 public:
    static discrete_distribution<int> createUniformIntGenerator(int num_elements);
};

#endif  // INCLUDE_TRANSFORMATIONS_RENAMING_DISTRIBUTIONGENERATOR_H_
