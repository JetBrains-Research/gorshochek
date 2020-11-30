#include <include/transformations/renaming/HashRenameProcessor.h>

using std::hash, std::to_string;
// ------------ HashRenameProcessor ------------

HashRenameProcessor::HashRenameProcessor(const string * hash_prefix) :
        hash_prefix(*hash_prefix) {}

string HashRenameProcessor::generateNewName(string * name) {
    return hash_prefix + "_" + to_string(hash<string>{}(*name));
}

BaseRenameProcessor * HashRenameProcessor::buildFromConfig(const YAML::Node & config) {
    // TODO: generate random hash prefix by default?
    const auto hash_prefix = config["hash prefix"] != nullptr ? config["hash prefix"].as<string>() : "d";
    return new HashRenameProcessor(&hash_prefix);
}
