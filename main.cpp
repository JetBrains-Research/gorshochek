#include <iostream>
#include <filesystem>

#include "include/Runner.h"
#include "include/Utils.h"

using std::size_t, std::cerr, std::endl, std::copy;
namespace fs = std::filesystem;

int main(int argc, const char **argv) {
    string config_path;

    if (argc == 3) {
        config_path = fs::current_path() / fs::path("config.yaml");
    } else if (argc == 4) {
        config_path = argv[3];
    } else {
        cerr << "Invalid number of positional"
             << "arguments specified" << endl;
        return 1;
    }
    string input_path = argv[1];
    string output_path = argv[2];

    // Parsing config to get original files paths and transformations for them
    const vector<ITransformation *> *transformations = getTransformationsFromYaml(config_path);
    // Getting number of transformations
    size_t n_transformations = getNumTransformationsFromYaml(config_path);
    size_t batch_size = getBatchSizeFromYaml(config_path);
    Runner(transformations, n_transformations, batch_size).run(input_path, output_path);
    return 0;
}
