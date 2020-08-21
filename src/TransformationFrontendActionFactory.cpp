#include "../include/TransformationFrontendActionFactory.h"

TransformationFrontendActionFactory::TransformationFrontendActionFactory(
        const vector<ITransformation *> *transformations,
        const string output_path,
        mt19937 *gen
) :
    transformations(transformations), output_path(output_path), gen(gen) {}

FrontendAction * TransformationFrontendActionFactory::create() {
    return new TransformationFrontendAction(transformations, output_path, gen);
}
