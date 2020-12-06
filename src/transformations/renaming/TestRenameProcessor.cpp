#include "include/transformations/renaming/TestRenameProcessor.h"

using std::hash, std::to_string;
// ------------ TestRenameProcessor ------------

TestRenameProcessor::TestRenameProcessor() = default;

string TestRenameProcessor::generateNewName(string * name) {
    return prefix + "_" + *name;
}

BaseRenameProcessor * TestRenameProcessor::buildFromConfig(const YAML::Node & config) {
    return new TestRenameProcessor();
}
