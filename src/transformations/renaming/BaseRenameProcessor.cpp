#include "include/transformations/renaming/BaseRenameProcessor.h"

using std::logic_error;
// ------------ BaseRenameProcessor ------------

string BaseRenameProcessor::generateNewName(string * name) {
    throw logic_error("This function is not implemented");
}

BaseRenameProcessor * buildFromConfig(const YAML::Node & config) {
    throw logic_error("This function is not implemented");
}

