#include "../../include/transformations/RemoveCommentsTransformation.h"

using clang::RawCommentList, clang::SourceManager, clang::RawComment;
using llvm::isa, llvm::cast;
using std::unique_ptr, std::find, std::vector, std::string;


// ------------ RemoveCommentsConsumer ------------

RemoveCommentsConsumer::RemoveCommentsConsumer(Rewriter * rewriter) : rewriter(rewriter) {}

void RemoveCommentsConsumer::HandleTranslationUnit(ASTContext &ctx) {
    SourceManager& sm = ctx.getSourceManager();
    RawCommentList & commentList = ctx.getRawCommentList();
    auto comments = commentList.getComments();

    for (auto const& comment: comments) {
        std::string raw = comment->getRawText(sm);
        std::string brief = comment->getBriefText(ctx);
        std::cout << raw << std::endl;
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