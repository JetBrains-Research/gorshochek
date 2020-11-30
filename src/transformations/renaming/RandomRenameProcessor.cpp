#include <include/transformations/renaming/RandomRenameProcessor.h>
#include <include/transformations/renaming/DistributionGenerator.h>

using std::hash, std::to_string;
// ------------ RandomRenameProcessor ------------

RandomRenameProcessor::RandomRenameProcessor(const int seed,
                                             const int max_tokens,
                                             const int max_token_len) {
    gen = new mt19937(seed);
    token_len_generator = DistributionGenerator::createUniformIntGenerator(max_token_len);
    tokens_num_generator = DistributionGenerator::createUniformIntGenerator(max_tokens);
    char_generator = DistributionGenerator::createUniformIntGenerator(num_lat_chars);
}

string RandomRenameProcessor::generateNewName(string * name) {
    string newName;
    int num_tokens = tokens_num_generator(*gen) + 1;
    for (int tok_idx = 0; tok_idx < num_tokens; ++tok_idx) {
        int token_len = token_len_generator(*gen) + 1;
        for (int ch_idx = 0; ch_idx < token_len; ++ch_idx) {
            char newChar = 'a' + char_generator(*gen);
            newName += newChar;
        }
        if (tok_idx != num_tokens - 1) {
            newName += "_";
        }
    }
    return newName;
}

BaseRenameProcessor * RandomRenameProcessor::buildFromConfig(const YAML::Node &config) {
    const auto seed = config["seed"].as<int>();
    const auto max_tokens = config["max tokens"].as<int>();
    const auto max_token_len = config["max token len"].as<int>();
    return new RandomRenameProcessor(seed, max_tokens, max_token_len);
}


