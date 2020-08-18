#include "../include/Utils.h"

#include <string>
#include <vector>

using std::string, std::vector;

vector<TransformationsType> getTransformations(string config_path){
    vector<TransformationsType> a = {{"a", "b"}, {"a", "b"}};
    return a;
}

vector<string> getPaths(string config_path){
    vector<string> a = {"a", "b", "a", "b"};
    return a;
}