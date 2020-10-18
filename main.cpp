#include <iostream>

#include "include/Runner.h"
#include "include/Utils.h"

using std::size_t, std::cerr, std::endl, std::copy;

int main(int argc, const char **argv) {
    if (argc < 3) {
        cerr << "Not enough positional command line"
             << "arguments specified" << endl;
    }
    string config_path = argv[1];
    // Parsing config to get original files paths and transformations for them
    const vector<ITransformation *> *transformations = getTransformationsFromYaml(config_path);
    // Getting output path where to save transformed code
    string output_path = getOutputPathFromYaml(config_path);
    // Getting number of transformations
    size_t n_transformations = getNumTransformationsFromYaml(config_path);
    Runner(transformations, n_transformations).run(argc - 2, const_cast<char *>(*argv), const_cast<char **>(argv + 2), output_path);
    return 0;
}
