#include "../include/TransformationFrontendActionFactory.h"

TransformationFrontendActionFactory::TransformationFrontendActionFactory(ITransformation * transformation) :
    transformation(transformation) {}

FrontendAction * TransformationFrontendActionFactory::create() {
    return new TransformationFrontendAction(transformation);
}
