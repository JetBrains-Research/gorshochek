#ifndef GORSHOCHEK_HASHRENAMEPROCESSOR_H
#define GORSHOCHEK_HASHRENAMEPROCESSOR_H

#include "BaseRenameProcessor.h"

class HashRenameProcessor : public BaseRenameProcessor {
    /**
     * Generate a new name by the following rule:
     * take the hash from the previous name with a certain prefix
     */

    explicit HashRenameProcessor(const string * hash_prefix);

    string generateNewName(string *name) override;

public:
    static BaseRenameProcessor * buildFromConfig(const YAML::Node & config);

private:
    const string hash_prefix;
};

#endif //GORSHOCHEK_HASHRENAMEPROCESSOR_H
