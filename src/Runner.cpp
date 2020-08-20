#include <yaml-cpp/yaml.h>

#include <string>
#include <vector>

#include "../include/Runner.h"
#include "../include/Utils.h"
#include "../include/TransformationFrontendAction.h"
#include "../include/TransformationFrontendActionFactory.h"

using clang::tooling::FrontendActionFactory,
clang::tooling::CommonOptionsParser, clang::tooling::ClangTool,
clang::FrontendAction;
using std::size_t, std::vector, std::string, std::ifstream;

Runner::Runner(int argc, const char **argv) : argc(argc), argv(argv) {}

void Runner::run() {
    string config_path = argv[1];
    // Parsing config to get original files paths and transformations for them
    vector<BaseTransformation> transformations = getTransformationsFromYaml(config_path);
    // Getting output path where to save transformed code
    YAML::Node config = YAML::LoadFile(config_path);
    auto output_path = config["output path"].as<string>();
    // Constructing specific input for CommonOptionsParser
    vector<const char *> args;
    for (int arg_index = 0; arg_index < argc; ++arg_index) {
        if (arg_index != 1)
            args.push_back(argv[arg_index]);
    }
    argc--;
    const char **argv_ = args.data();

    auto OptionsParser = CommonOptionsParser(argc, argv_, TransformationCategory);
    // Constructs a clang tool to run over a list of files.
    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());
    // Run the Clang Tool, creating a new FrontendAction
    Tool.run(std::unique_ptr<FrontendActionFactory>(
                new TransformationFrontendActionFactory(
                    move(transformations), move(output_path)
                )
            ).get()
    );
}


