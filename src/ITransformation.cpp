#include "../include/ITransformation.h"

// ------------ ITransformation ------------

ITransformation::ITransformation(float p) : p(p) {}

ITransformation::~ITransformation() {}

float ITransformation::getProbability() {
    return p;
}

string ITransformation::getName() {
    return string(name);
}
