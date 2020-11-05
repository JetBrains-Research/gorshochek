#include "../../include/transformations/RemoveCommentsTransformation.h"

using clang::RawCommentList, clang::SourceManager, clang::RawComment;
using llvm::isa, llvm::cast;
using std::unique_ptr, std::find, std::vector, std::string;


// ------------ RemoveCommentsASTConsumer ------------

RemoveCommentsASTConsumer::RemoveCommentsASTConsumer(Rewriter * rewriter) : rewriter(rewriter) {}

void RemoveCommentsASTConsumer::HandleTranslationUnit(ASTContext &ctx) {
    RawCommentList & commentList = ctx.getRawCommentList();
    for (auto const & comment : commentList.getComments()) {
        rewriter->RemoveText(comment->getSourceRange());
    }
    rewriter->overwriteChangedFiles();
}

// ------------ RemoveCommentsTransformation ------------

RemoveCommentsTransformation::RemoveCommentsTransformation(const float p) :
                                                           ITransformation(p, "remove comments") {}

unique_ptr<ASTConsumer> RemoveCommentsTransformation::getConsumer(Rewriter * rewriter) {
    return llvm::make_unique<RemoveCommentsASTConsumer>(rewriter);
}

ITransformation * RemoveCommentsTransformation::buildFromConfig(const YAML::Node & config) {
    const auto p = config["p"].as<float>();
    return new RemoveCommentsTransformation(p);
}
