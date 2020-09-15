#include "../include/ITransformation.h"

using std::move;
// ------------ ITransformation ------------

ITransformation::ITransformation(const float p, const string name) :
        p(p), name(move(name)) {}

float ITransformation::getProbability() const {
    return p;
}

string const & ITransformation::getName() const {
    return name;
}
