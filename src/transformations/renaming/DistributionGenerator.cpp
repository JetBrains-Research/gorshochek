#include "include/transformations/renaming/DistributionGenerator.h"

using std::vector;

discrete_distribution<int> DistributionGenerator::createUniformIntGenerator(const int num_elements) {
    /**
     * Such a strange way to generate random variables is essential to guarantee
     * the same behaviour of random module on different OS.
     * This way of generated random values is suggested in the following post:
     * http://anadoxin.org/blog/c-shooting-yourself-in-the-foot-4.html
     */
    auto probabilities = vector<double>(num_elements);
    for (int i = 0; i < num_elements; ++i) {
        // Here we create a discrete uniform distribution with probability
        // equals to 100 / num_elements
        probabilities[i] = 100.0 / num_elements;
    }
    return discrete_distribution<int> (probabilities.begin(), probabilities.end());
}

