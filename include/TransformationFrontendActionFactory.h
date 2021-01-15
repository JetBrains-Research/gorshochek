#ifndef INCLUDE_TRANSFORMATIONFRONTENDACTIONFACTORY_H_
#define INCLUDE_TRANSFORMATIONFRONTENDACTIONFACTORY_H_

#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"

#include "../include/Utils.h"
#include "../include/TransformationFrontendAction.h"

using clang::tooling::FrontendActionFactory, clang::FrontendAction;

/// This class is a factory to create new TransformationFrontendActions. We need this class since the default
/// FrontedActionFactory (https://clang.llvm.org/doxygen/classclang_1_1tooling_1_1FrontendActionFactory.html)
/// doesn't accept any parameters for FrontendAction
class TransformationFrontendActionFactory : public FrontendActionFactory {
 public:
    explicit TransformationFrontendActionFactory(ITransformation * transformation);
    FrontendAction * create() override;
 private:
    ITransformation * transformation;
};

#endif  // INCLUDE_TRANSFORMATIONFRONTENDACTIONFACTORY_H_
