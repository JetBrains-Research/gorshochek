#include "../include/TransformationFrontendActionFactory.h"

TransformationFrontendActionFactory::TransformationFrontendActionFactory(vector<BaseTransformation> transformations, string output_path):
    transformations(move(transformations)), output_path(move(output_path)) {};

FrontendAction * TransformationFrontendActionFactory::create() {
    return new TransformationFrontendAction(transformations, output_path);
}
