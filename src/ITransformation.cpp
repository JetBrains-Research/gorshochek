#include "../include/ITransformation.h"

using std::move;
// ------------ ITransformation ------------

ITransformation::ITransformation(const YAML::Node & config, string name) : name(move(name)) {
    p = config["p"].as<float>();
}

float ITransformation::getProbability() const {
    return p;
}

string const & ITransformation::getName() const {
    return name;
}
