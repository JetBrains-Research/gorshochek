#ifndef INCLUDE_TRANSFORMATIONFRONTENDACTIONFACTORY_H_
#define INCLUDE_TRANSFORMATIONFRONTENDACTIONFACTORY_H_

#include <string>
#include <vector>

#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"

#include "../include/Utils.h"
#include "../include/TransformationFrontendAction.h"

using clang::tooling::FrontendActionFactory, clang::FrontendAction;

/// This class is a factory to create new TransformationFrontendActions. The need for this class
/// arouse since newFrontendActionFactory (https://clang.llvm.org/doxygen/Tooling_8h_source.html)
/// function does not accept FrontendActions having specific constructor like
/// TransformationFrontendActions
class TransformationFrontendActionFactory : public FrontendActionFactory {
 public:
    TransformationFrontendActionFactory(
            const vector<ITransformation *> *transformations,
            const string output_path,
            mt19937 *gen);
    FrontendAction * create() override;
 private:
    const vector<ITransformation *> *transformations;
    const string output_path;
    mt19937 *gen;
};

#endif  // INCLUDE_TRANSFORMATIONFRONTENDACTIONFACTORY_H_
