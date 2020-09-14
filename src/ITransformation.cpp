#include "../include/ITransformation.h"

using std::move;
// ------------ ITransformation ------------

ITransformation::ITransformation(const YAML::Node & config, string name) :
        p(config["p"].as<float>()), name(move(name)) {}

float ITransformation::getProbability() const {
    return p;
}

string const & ITransformation::getName() const {
    return name;
}
