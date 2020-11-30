#ifndef INCLUDE_TRANSFORMATIONS_RENAMING_BASERENAMEPROCESSOR_H_
#define INCLUDE_TRANSFORMATIONS_RENAMING_BASERENAMEPROCESSOR_H_

#include <yaml-cpp/yaml.h>

#include <string>

using std::string;

class BaseRenameProcessor {
    /**
     * Base class for all renaming processors, which generates a new name
     */
 public:
    BaseRenameProcessor() = default;
    virtual ~BaseRenameProcessor() = default;

    virtual string generateNewName(string * name);

    static BaseRenameProcessor * buildFromConfig(const YAML::Node & config);
};

#endif  // INCLUDE_TRANSFORMATIONS_RENAMING_BASERENAMEPROCESSOR_H_
