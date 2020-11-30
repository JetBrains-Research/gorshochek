#ifndef GORSHOCHEK_TESTRENAMEPROCESSOR_H
#define GORSHOCHEK_TESTRENAMEPROCESSOR_H

#include "BaseRenameProcessor.h"

class TestRenameProcessor : public BaseRenameProcessor {
    /**
     * Generate a new name by the following rule:
     * add prefix "test" into the old name
     */

    explicit TestRenameProcessor();

    string generateNewName(string * name) override;

public:
    static BaseRenameProcessor * buildFromConfig(const YAML::Node &config);

private:
    const string prefix = "test";
};

#endif //GORSHOCHEK_TESTRENAMEPROCESSOR_H
