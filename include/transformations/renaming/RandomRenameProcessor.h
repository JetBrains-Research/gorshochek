#ifndef GORSHOCHEK_RANDOMRENAMEPROCESSOR_H
#define GORSHOCHEK_RANDOMRENAMEPROCESSOR_H

#include <random>

#include "BaseRenameProcessor.h"

using std::mt19937, std::discrete_distribution;

class RandomRenameProcessor : public BaseRenameProcessor {
    /**
     * Generate a new name by the following rule:
     * take the hash from the previous name with a certain prefix
     */

    explicit RandomRenameProcessor(int seed,
                                   int max_tokens,
                                   int max_token_len);

    string generateNewName(string *name) override;

public:
    static BaseRenameProcessor * buildFromConfig(const YAML::Node & config);

private:
    discrete_distribution<int> token_len_generator;
    discrete_distribution<int> tokens_num_generator;
    discrete_distribution<int> char_generator;

    mt19937 * gen;

    const int num_lat_chars = 26;
};

#endif //GORSHOCHEK_RANDOMRENAMEPROCESSOR_H
