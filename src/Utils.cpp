#include <yaml-cpp/yaml.h>

#include <iostream>
#include <string>
#include <vector>

#include "../include/Utils.h"
#include "../include/transformations/IdentityTransformation.h"
#include "../include/TransformationFrontendActionFactory.h"

using std::string, std::cerr, std::endl;

const char * CONFIG_TRANSFORMATIONS_KEY = "transformations";
const char * CONFIG_OUTPUT_PATH_KEY = "output path";


vector<ITransformation *> getTransformationsFromYaml(const string &config_path) {
    YAML::Node config = YAML::LoadFile(config_path);
    vector<ITransformation *> transformations;
    for (auto transform_data : config[CONFIG_TRANSFORMATIONS_KEY]) {
        if (transform_data["identity transform"]) {
            auto p = transform_data["identity transform"]["p"].as<float>();
            transformations.push_back(new IdentityTransformation(p));
        } else {
            cerr << "Unknown transformation" << endl;
        }
    }
    return transformations;
}

string getOutputPathFromYaml(const string &config_path) {
    YAML::Node config = YAML::LoadFile(config_path);
    return config[CONFIG_OUTPUT_PATH_KEY].as<string>();
}
