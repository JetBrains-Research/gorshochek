#include <yaml-cpp/yaml.h>

#include <iostream>
#include <string>
#include <vector>

#include "../include/Utils.h"
#include "../include/Common.h"

using clang::tooling::runToolOnCode, clang::tooling::FrontendActionFactory,
    clang::tooling::CommonOptionsParser, clang::tooling::ClangTool,
    clang::FrontendAction;
using std::size_t, std::vector, std::string, std::cerr, std::cout, std::endl,
    std::ifstream;

std::unique_ptr<FrontendActionFactory>
newBaseFrontendActionFactory(vector<BaseTransformation> transformations,
                             string output_path) {
    /* Method is similar to newFrontendActionFactory but this method
     * supports passing arguments to the FrontendAction constructor */
    class SimpleFrontendActionFactory : public FrontendActionFactory {
     public:
        SimpleFrontendActionFactory(vector<BaseTransformation> transformations, string output_path):
        transformations(move(transformations)), output_path(move(output_path)) {}
        FrontendAction *create() override {
            return new BaseFrontendAction(transformations, output_path);
        }
     private:
        vector<BaseTransformation> transformations;
        string output_path;
    };

    return std::unique_ptr<FrontendActionFactory>(
            new SimpleFrontendActionFactory(move(transformations), move(output_path)));
}

Runner::Runner(int argc, const char **argv) : argc(argc), argv(argv) {}

vector<BaseTransformation>
Runner::getTransformations(const string &config_path) {
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

void Runner::run() {
  string config_path = argv[1];
  // Parsing config to get original files paths and transformations for them
  vector<BaseTransformation> transformations = getTransformations(config_path);
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
  Tool.run(newBaseFrontendActionFactory(transformations, output_path).get());
}
