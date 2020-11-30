#ifndef GORSHOCHEK_DISCRETEDISTRIBUTIONGENERATOR_H
#define GORSHOCHEK_DISCRETEDISTRIBUTIONGENERATOR_H

#include <random>

using std::discrete_distribution;

static class DistributionGenerator {

public:
    static discrete_distribution<int> createUniformIntGenerator(int num_elements);
};

#endif //GORSHOCHEK_DISCRETEDISTRIBUTIONGENERATOR_H
