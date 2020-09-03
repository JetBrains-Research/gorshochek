#include <yaml-cpp/yaml.h>

#include <iostream>
#include <string>
#include <vector>

#include "../include/Utils.h"
#include "../include/transformations/IdentityTransformation.h"
#include "../include/transformations/AddCommentsTransformation.h"
#include "../include/transformations/RemoveCommentsTransformation.h"
#include "../include/transformations/RenameEntitiesTransformation.h"
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
            auto identity_config = transform_data["identity transform"];
            auto p = identity_config["p"].as<float>();
            transformations->push_back(new IdentityTransformation(p));
        } else if (transform_data["add comments"]) {
            auto add_comments_config = transform_data["add comments"];
            auto p = add_comments_config["p"].as<float>();
            const auto statements = new vector<string>;
            for (auto stmt : add_comments_config["statements"]) {
                statements->push_back(stmt.as<string>());
            }
            transformations->push_back(new AddCommentsTransformation(p, statements));
        } else if (transform_data["rename entities"]) {
            auto rename_config = transform_data["rename entities"];
            auto p = rename_config["p"].as<float>();
            auto seed = rename_config["seed"].as<int>();
            auto max_tokens = rename_config["max tokens"].as<int>();
            auto max_token_len = rename_config["max token len"].as<int>();
            bool rename_func = false, rename_var = false, test = false;
            if (rename_config["rename functions"]) {
                rename_func = rename_config["rename functions"].as<bool>();
            }
            if (rename_config["rename variables"]) {
                rename_var = rename_config["rename variables"].as<bool>();
            }
            if (rename_config["test"]) {
                test = rename_config["test"].as<bool>();
            }
            transformations->push_back(new RenameEntitiesTransformation(p, rename_func, rename_var, seed,
                                                                        max_tokens, max_token_len, test));
        } else if (transform_data["remove comments"]) {
            auto remove_comments_config = transform_data["remove comments"];
            auto p = remove_comments_config["p"].as<float>();
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
