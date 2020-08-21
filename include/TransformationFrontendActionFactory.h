#ifndef INCLUDE_TRANSFORMATIONFRONTENDACTIONFACTORY_H_
#define INCLUDE_TRANSFORMATIONFRONTENDACTIONFACTORY_H_

#include <string>
#include <vector>

#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"

#include "../include/Utils.h"
#include "../include/TransformationFrontendAction.h"

using clang::tooling::FrontendActionFactory, clang::FrontendAction;

class TransformationFrontendActionFactory : public FrontendActionFactory {
 public:
    TransformationFrontendActionFactory(
            vector<ITransformation *> transformations,
            string output_path,
            mt19937 gen);
    FrontendAction * create() override;
 private:
    vector<ITransformation *> transformations;
    string output_path;
    mt19937 gen;
};

#endif  // INCLUDE_TRANSFORMATIONFRONTENDACTIONFACTORY_H_
