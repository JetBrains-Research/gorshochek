#ifndef INCLUDE_TRANSFORMATIONFONTENDACTIONFACTORY_H_
#define INCLUDE_TRANSFORMATIONFONTENDACTIONFACTORY_H_

#include <string>
#include <vector>

#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"

#include "../include/Utils.h"
#include "../include/TransformationFrontendAction.h"

using clang::tooling::FrontendActionFactory, clang::FrontendAction;

class TransformationFrontendActionFactory : public FrontendActionFactory {
public:
    TransformationFrontendActionFactory(vector<BaseTransformation> transformations, string output_path);
    FrontendAction * create() override;
private:
    vector<BaseTransformation> transformations;
    string output_path;
};

#endif  //INCLUDE_TRANSFORMATIONFONTENDACTIONFACTORY_H_
