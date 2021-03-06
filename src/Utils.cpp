#include <yaml-cpp/yaml.h>

#include <iostream>
#include <string>
#include <vector>

#include "../include/Utils.h"
#include "../include/transformations/IdentityTransformation.h"
#include "../include/transformations/AddCommentsTransformation.h"
#include "../include/transformations/RemoveCommentsTransformation.h"
#include "../include/transformations/RenameEntitiesTransformation.h"
#include "../include/transformations/ReorderFuncDeclsTransformation.h"
#include "../include/transformations/IfElseSwapTransformation.h"
#include "../include/transformations/input_output/PrintfToCoutTransformation.h"
#include "../include/transformations/ForToWhileTransformation.h"
#include "../include/transformations/WhileToForTransformation.h"
#include "../include/transformations/OpenMacrosTransformation.h"
#include "../include/TransformationFrontendActionFactory.h"

using std::string, std::function, std::cerr, std::endl, std::size_t;

const char * CONFIG_TRANSFORMATIONS_KEY = "transformations";
const char * CONFIG_NUM_TRANSFORMATIONS_KEY = "n transformations";
const char * CONFIG_BATCH_SIZE_KEY = "batch size";
const char * CONFIG_LOGGING_KEY = "logging";
size_t DEFAULT_BATCH_SIZE = 16;
bool DEFAULT_LOGGING_FLAG = true;

const map<string, function<ITransformation *(const YAML::Node &)>> transformFactory = {
        {"identity transform", IdentityTransformation::buildFromConfig },
        {"add comments", AddCommentsTransformation::buildFromConfig },
        {"remove comments", RemoveCommentsTransformation::buildFromConfig },
        {"rename entities", RenameEntitiesTransformation::buildFromConfig },
        {"reorder function decls", ReorderFuncDeclsTransformation::buildFromConfig },
        {"if else swap", IfElseSwapTransformation::buildFromConfig },
        {"printf to cout", PrintfToCoutTransformation::buildFromConfig },
        {"for to while", ForToWhileTransformation::buildFromConfig },
        {"while to for", WhileToForTransformation::buildFromConfig },
        {"open macros", OpenMacrosTransformation::buildFromConfig }
};

vector<ITransformation *> *getTransformationsFromYaml(const string &config_path) {
    YAML::Node config = YAML::LoadFile(config_path);
    if (!config[CONFIG_TRANSFORMATIONS_KEY]) {
        cerr << "Specify transformations in yaml config inside \""
             << CONFIG_TRANSFORMATIONS_KEY
             << "\" key" << endl;
    }
    auto transformations = new vector<ITransformation *>;
    for (const auto & transform_data : config[CONFIG_TRANSFORMATIONS_KEY]) {
        bool isTransformUnknown = true;
        for (const auto & transform : transformFactory) {
            auto transform_name = transform.first;
            if (transform_data[transform_name]) {
                transformations->push_back(transform.second(transform_data[transform_name]));
                isTransformUnknown = false;
            }
        }
        if (isTransformUnknown) {
            cerr << "Unknown transformation";
        }
    }
    return transformations;
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

size_t getBatchSizeFromYaml(const string &config_path) {
    YAML::Node config = YAML::LoadFile(config_path);
    return config[CONFIG_BATCH_SIZE_KEY] != nullptr ? config[CONFIG_BATCH_SIZE_KEY].as<size_t>() : DEFAULT_BATCH_SIZE;
}

bool getLoggingFlagFromYaml(const string &config_path) {
    YAML::Node config = YAML::LoadFile(config_path);
    return config[CONFIG_LOGGING_KEY] != nullptr ? config[CONFIG_LOGGING_KEY].as<bool>() : DEFAULT_LOGGING_FLAG;
}
