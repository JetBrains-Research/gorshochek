#include <iostream>

#include "include/Runner.h"
#include "include/Utils.h"

using std::cerr, std::endl;

int main(int argc, const char **argv) {
    if (argc < 3)
        cerr << "Not enough positional command line"
             << "arguments specified" << endl;
    string config_path = argv[1];
    // Parsing config to get original files paths and transformations for them
    vector<ITransformation *> transformations = getTransformationsFromYaml(config_path);
    // Getting output path where to save transformed code
    string output_path = getOutputPathFromYaml(config_path);
    // Excluding config_path from argv to pass it as input for CommonOptionsParser
    vector<const char *> args;
    for (int arg_index = 0; arg_index < argc; ++arg_index) {
        if (arg_index != 1)
            args.push_back(argv[arg_index]);
    }
    int num_files = argc - 1;
    const char ** files = args.data();
    Runner(transformations).run(num_files, files, output_path);
    return 0;
}
