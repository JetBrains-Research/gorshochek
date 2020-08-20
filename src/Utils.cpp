#include <yaml-cpp/yaml.h>

#include <iostream>
#include <string>
#include <vector>

#include "../include/Utils.h"
#include "../include/TransformationFrontendActionFactory.h"

using std::cerr, std::endl;

vector<BaseTransformation> getTransformationsFromYaml(const string &config_path) {
    YAML::Node config = YAML::LoadFile(config_path);
    vector<BaseTransformation> transformations;
    for (auto transform_data : config["transformations"]) {
        if (transform_data["basic transform"]) {
            auto p = transform_data["basic transform"]["p"].as<float>();
            transformations.emplace_back(BaseTransformation(p));
        } else {
            cerr << "Unknown transformation" << endl;
        }
    }
    return transformations;
}