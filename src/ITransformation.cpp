#include "../include/ITransformation.h"

// ------------ ITransformation ------------

ITransformation::ITransformation(float p) : p(p) {}

ITransformation::~ITransformation() {}

float ITransformation::getProbability() {
    return p;
}
