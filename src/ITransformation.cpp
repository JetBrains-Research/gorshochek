#include "../include/ITransformation.h"

using std::move, std::logic_error;

// ------------ ITransformation ------------

ITransformation::ITransformation(const float p, const string name) :
        p(p), name(move(name)) {}

float ITransformation::getProbability() const {
    return p;
}

string const & ITransformation::getName() const {
    return name;
}

ITransformation * buildFromConfig(const YAML::Node & config, const string name) {
    throw logic_error("This function is not implemented");
}
