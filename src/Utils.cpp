#include <yaml-cpp/yaml.h>

#include <iostream>
#include <string>
#include <vector>

#include "../include/Utils.h"
#include "../include/transformations/IdentityTransformation.h"
#include "../include/transformations/AddCommentsTransformation.h"
#include "../include/transformations/RemoveCommentsTransformation.h"
#include "../include/TransformationFrontendActionFactory.h"

using std::string, std::cerr, std::endl, std::size_t;

const char * CONFIG_TRANSFORMATIONS_KEY = "transformations";
const char * CONFIG_OUTPUT_PATH_KEY = "output path";
const char * CONFIG_NUM_TRANSFORMATIONS_KEY = "n transformations";


vector<ITransformation *> *getTransformationsFromYaml(const string &config_path) {
    YAML::Node config = YAML::LoadFile(config_path);
    if (!config[CONFIG_TRANSFORMATIONS_KEY]) {
        cerr << "Specify transformations in yaml config inside \""
             << CONFIG_TRANSFORMATIONS_KEY
             << "\" key" << endl;
    }
    auto transformations = new vector<ITransformation *>;
    for (auto transform_data : config[CONFIG_TRANSFORMATIONS_KEY]) {
        if (transform_data["identity transform"]) {
            auto p = transform_data["identity transform"]["p"].as<float>();
            transformations->push_back(new IdentityTransformation(p));
        } else if (transform_data["add comments"]) {
            auto p = transform_data["add comments"]["p"].as<float>();
            vector<string> statements;
            for (auto stmt : transform_data["add comments"]["statements"]) {
                statements.push_back(stmt.as<string>());
            }
            transformations->push_back(new AddCommentsTransformation(p, statements));
        } else if (transform_data["remove comments"]) {
            auto p = transform_data["remove comments"]["p"].as<float>();
            transformations->push_back(new RemoveCommentsTransformation(p));
        } else {
            cerr << "Unknown transformation";
        }
    }
    return transformations;
}

string getOutputPathFromYaml(const string &config_path) {
    YAML::Node config = YAML::LoadFile(config_path);
    if (!config[CONFIG_OUTPUT_PATH_KEY]) {
        cerr << "Specify output path in yaml config inside \""
             << CONFIG_OUTPUT_PATH_KEY
             << "\" key" << endl;
    }
    return config[CONFIG_OUTPUT_PATH_KEY].as<string>();
}

size_t getNumTransformationsFromYaml(const string &config_path) {
    YAML::Node config = YAML::LoadFile(config_path);
    if (!config[CONFIG_NUM_TRANSFORMATIONS_KEY]) {
        cerr << "Specify num transformations in yaml config inside \""
             << CONFIG_NUM_TRANSFORMATIONS_KEY
             << "\" key" << endl;
    }
    return config[CONFIG_NUM_TRANSFORMATIONS_KEY].as<size_t>();
}
