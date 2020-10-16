#include "../include/TransformationFrontendActionFactory.h"

TransformationFrontendActionFactory::TransformationFrontendActionFactory(
        const vector<ITransformation *> *transformations,
        string const & output_path,
        mt19937 *gen
) :
    transformations(transformations), output_path(output_path), gen(gen) {}

std::unique_ptr<FrontendAction> TransformationFrontendActionFactory::create() {
    auto ptr = new TransformationFrontendAction(transformations, output_path, gen);
    return std::unique_ptr<FrontendAction>(ptr);
}
