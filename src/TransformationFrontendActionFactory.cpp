#include "../include/TransformationFrontendActionFactory.h"

TransformationFrontendActionFactory::TransformationFrontendActionFactory(
        vector<ITransformation *> transformations, string output_path, mt19937 gen
) :
    transformations(move(transformations)), output_path(move(output_path)), gen(gen) {}

FrontendAction * TransformationFrontendActionFactory::create() {
    return new TransformationFrontendAction(transformations, output_path, gen);
}
