#ifndef GORSHOCHEK_BASERENAMEPROCESSOR_H
#define GORSHOCHEK_BASERENAMEPROCESSOR_H

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

#endif // GORSHOCHEK_BASERENAMEPROCESSOR_H
