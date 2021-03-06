#ifndef INCLUDE_TRANSFORMATIONS_RENAMING_RANDOMRENAMEPROCESSOR_H_
#define INCLUDE_TRANSFORMATIONS_RENAMING_RANDOMRENAMEPROCESSOR_H_

#include <random>
#include <string>

#include "BaseRenameProcessor.h"

using std::mt19937, std::discrete_distribution;

class RandomRenameProcessor : public BaseRenameProcessor {
    /**
     * Generate a random new name
     */
 public:
    explicit RandomRenameProcessor(int seed,
                                   int max_tokens,
                                   int max_token_len);

    string generateNewName(string * name) override;
    static BaseRenameProcessor * buildFromConfig(const YAML::Node &config);
 private:
    discrete_distribution<int> token_len_generator;
    discrete_distribution<int> tokens_num_generator;
    discrete_distribution<int> char_generator;

    mt19937 *gen;

    const int num_lat_chars = 26;
};

#endif  // INCLUDE_TRANSFORMATIONS_RENAMING_RANDOMRENAMEPROCESSOR_H_
