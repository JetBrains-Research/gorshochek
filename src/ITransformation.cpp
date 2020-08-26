#include "../include/ITransformation.h"

using std::move;
// ------------ ITransformation ------------

ITransformation::ITransformation(float p, string name) : p(p), name(move(name)) {}

ITransformation::~ITransformation() {}

float ITransformation::getProbability() const {
    return p;
}

string const & ITransformation::getName() const {
    return name;
}
