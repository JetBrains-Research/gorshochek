#include "../../include/transformations/RemoveCommentsTransformation.h"

using clang::RawCommentList, clang::SourceManager, clang::RawComment;
using llvm::isa, llvm::cast;
using std::unique_ptr, std::find, std::vector, std::string;


// ------------ RemoveCommentsConsumer ------------

RemoveCommentsConsumer::RemoveCommentsConsumer(Rewriter * rewriter) : rewriter(rewriter) {}

void RemoveCommentsConsumer::HandleTranslationUnit(ASTContext &ctx) {
    RawCommentList & commentList = ctx.getRawCommentList();
    for (auto const & comment : commentList.getComments()) {
        rewriter->RemoveText(comment->getSourceRange());
    }
}

// ------------ RemoveCommentsTransformation ------------

RemoveCommentsTransformation::RemoveCommentsTransformation(float p) :
                                                           ITransformation(p, "remove comments") {}

RemoveCommentsTransformation::~RemoveCommentsTransformation() {}

unique_ptr<ASTConsumer> RemoveCommentsTransformation::getConsumer(Rewriter * rewriter) {
    return llvm::make_unique<RemoveCommentsConsumer>(rewriter);
}
