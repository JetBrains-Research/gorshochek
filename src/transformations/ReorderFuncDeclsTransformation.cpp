#include "../../include/transformations/ReorderFuncDeclsTransformation.h"

using llvm::isa, llvm::cast;
using std::unique_ptr, std::sort, std::shuffle, std::find, std::reverse,
std::vector, std::string;
using clang::SourceRange, clang::SourceManager, clang::FileID, clang::CXXMethodDecl;

ReorderFuncDeclsVisitor::ReorderFuncDeclsVisitor(Rewriter * rewriter, mt19937 * gen, const bool test) :
                                                 rewriter(rewriter), sm(rewriter->getSourceMgr()),
                                                 gen(gen), test(test) {}

bool ReorderFuncDeclsVisitor::VisitFunctionDecl(FunctionDecl * decl) {
    if (!isFuncDeclProcessed(decl)) {
        auto loc = decl->getBeginLoc();
        if (decl->isThisDeclarationADefinition()
            && sm.isWrittenInMainFile(loc)
            && !(isa<CXXMethodDecl>(decl))) {
            funcdecls.push_back(decl);
        }
    }
    return true;
}

bool ReorderFuncDeclsVisitor::isFuncDeclProcessed(FunctionDecl * decl) {
    return find(funcdecls.begin(), funcdecls.end(), decl) != funcdecls.end();
}

bool ReorderFuncDeclsVisitor::compare(FunctionDecl * a, FunctionDecl * b) {
    auto a_name = a->getNameAsString();
    auto b_name = b->getNameAsString();
    return a_name < b_name;
}

void ReorderFuncDeclsVisitor::rewriteFunctions() {
    if (!funcdecls.empty()) {
        if (!test) {
            shuffle(funcdecls.begin(), funcdecls.end(), *gen);
        } else {
            sort(funcdecls.begin(), funcdecls.end(), compare);
        }
        SourceManager & sm = rewriter->getSourceMgr();
        SourceLocation endFile;
        FileID fid;
        for (auto &funcdecl : funcdecls) {
            fid = sm.getFileID(funcdecls[0]->getLocation());
            endFile = sm.getLocForEndOfFile(fid);
            rewriter->ReplaceText(endFile, "\n\n");
            endFile = sm.getLocForEndOfFile(fid);
            rewriter->ReplaceText(endFile, funcdecl->getSourceRange());
            if (funcdecl->isFirstDecl()) {
                SourceRange funcDeclOldRange = funcdecl->getBody()->getSourceRange();
                rewriter->ReplaceText(funcDeclOldRange, ";");
            } else {
                SourceRange funcDeclOldRange = funcdecl->getSourceRange();
                rewriter->RemoveText(funcDeclOldRange);
            }
        }
        rewriter->ReplaceText(endFile, "\n\n");
    }
}

ReorderFuncDeclsASTConsumer::ReorderFuncDeclsASTConsumer(Rewriter * rewriter, mt19937 * gen, const bool test) :
                                                         visitor(rewriter, gen, test), rewriter(rewriter) {}

void ReorderFuncDeclsASTConsumer::HandleTranslationUnit(ASTContext &ctx) {
    visitor.TraverseDecl(ctx.getTranslationUnitDecl());
    visitor.rewriteFunctions();
    rewriter->overwriteChangedFiles();
}

ReorderFuncDeclsTransformation::ReorderFuncDeclsTransformation(const float p, const int seed, const bool test) :
        ITransformation(p, "reorder function decls"),
        gen(new mt19937(seed)),
        test(test) {}

unique_ptr<ASTConsumer> ReorderFuncDeclsTransformation::getConsumer(Rewriter *rewriter) {
    return llvm::make_unique<ReorderFuncDeclsASTConsumer>(rewriter, gen, test);
}

ITransformation * ReorderFuncDeclsTransformation::buildFromConfig(const YAML::Node & config) {
    const auto p = config["p"].as<float>();
    const auto seed = config["seed"].as<int>();
    const auto test = config["test"] != nullptr && config["test"].as<bool>();
    return new ReorderFuncDeclsTransformation(p, seed, test);
}
