#include "../../include/transformations/RenameEntitiesTransformation.h"

#include <utility>
#include <iostream>
#include "include/transformations/renaming/BaseRenameProcessor.h"
#include "include/transformations/renaming/RandomRenameProcessor.h"
#include "include/transformations/renaming/TestRenameProcessor.h"
#include "include/transformations/renaming/HashRenameProcessor.h"

using llvm::isa, llvm::cast;
using std::unique_ptr, std::find, std::vector, std::string, std::to_string, std::hash, std::function;
using clang::DeclStmt, clang::FunctionDecl, clang::VarDecl, clang::DeclRefExpr,
clang::NamedDecl;

// ------------ RenameEntitiesVisitor ------------

RenameEntitiesVisitor::RenameEntitiesVisitor(Rewriter * rewriter, const bool rename_func, const bool rename_var,
                                             BaseRenameProcessor * processor):
        rewriter(rewriter), sm(rewriter->getSourceMgr()),
        rename_func(rename_func), rename_var(rename_var),
        processor(processor)  {
}

bool RenameEntitiesVisitor::VisitFunctionDecl(FunctionDecl * fdecl) {
    if (rename_func) {
        auto loc = fdecl->getBeginLoc();
        if (sm.isWrittenInMainFile(loc) && !(fdecl->isMain()) && !(fdecl->isOverloadedOperator())) {
            string name = fdecl->getNameInfo().getName().getAsString();
            if (!(fdecl->isTemplated()) && (decl2name.find(fdecl->getCanonicalDecl()) == decl2name.end())) {
                processVarDecl(fdecl->getCanonicalDecl(), &name);
            }
        }
    }
    return true;
}

bool RenameEntitiesVisitor::VisitStmt(Stmt * stmt) {
    if (rename_var) {
        if (isa<DeclRefExpr>(stmt)) {
            auto * de = cast<DeclRefExpr>(stmt);
            auto * decl = de->getDecl();
            auto loc = decl->getBeginLoc();
            if (isa<VarDecl>(decl) && sm.isWrittenInMainFile(loc)) {
                string name = de->getNameInfo().getAsString();
                if (decl2name.find(decl) == decl2name.end()) {
                    processVarDecl(decl, &name);
                }
                if (find(processed.begin(), processed.end(), stmt) == processed.end()) {
                    rewriter->ReplaceText(de->getExprLoc(), name.length(), decl2name.at(decl));
                    processed.push_back(stmt);
                }
            }
        }
        if (isa<DeclStmt>(stmt)) {
            auto * de = cast<DeclStmt>(stmt);
            if (de->isSingleDecl()) {
                auto * decl = de->getSingleDecl();
                auto loc = decl->getBeginLoc();
                if (isa<VarDecl>(decl) && sm.isWrittenInMainFile(loc)) {
                    auto * vardecl = cast<VarDecl>(decl);
                    string name = vardecl->getNameAsString();
                    if (decl2name.find(decl) == decl2name.end()) {
                        processVarDecl(decl, &name);
                    }
                }
            }
        }
    }
    return true;
}

bool RenameEntitiesVisitor::VisitCallExpr(CallExpr * call) {
    if (rename_func && call->getDirectCallee()) {
        FunctionDecl * fdecl  = call->getDirectCallee();
        auto loc = fdecl->getBeginLoc();
        if (sm.isWrittenInMainFile(loc) && !(fdecl->isMain()) && !(fdecl->isOverloadedOperator())) {
            string name = fdecl->getNameInfo().getName().getAsString();
            if (decl2name.find(fdecl->getCanonicalDecl()) == decl2name.end()) {
                processVarDecl(fdecl->getCanonicalDecl(), &name);
            }
            if (find(processed.begin(), processed.end(), cast<Stmt>(call)) == processed.end()) {
                rewriter->ReplaceText(call->getExprLoc(), name.length(), decl2name.at(fdecl));
                processed.push_back(cast<Stmt>(call));
            }
        }
    }
    return true;
}

void RenameEntitiesVisitor::processVarDecl(Decl * decl, string * name) {
    string randomName = processor->generateNewName(name);
    rewriter->ReplaceText(decl->getLocation(), name->length(), randomName);
    decl2name[decl] = randomName;
}

// ------------ RenameEntitiesASTConsumer ------------

RenameEntitiesASTConsumer::RenameEntitiesASTConsumer(Rewriter * rewriter, const bool rename_func, const bool rename_var,
                                                     BaseRenameProcessor * processor):
        visitor(rewriter, rename_func, rename_var, processor),
        rewriter(rewriter) {}

void RenameEntitiesASTConsumer::HandleTranslationUnit(ASTContext &ctx) {
    visitor.TraverseDecl(ctx.getTranslationUnitDecl());
    rewriter->overwriteChangedFiles();
}

// ------------ RenameEntitiesTransformation ------------

RenameEntitiesTransformation::RenameEntitiesTransformation(const float p, const bool rename_func,
                                                           const bool rename_var,
                                                           BaseRenameProcessor * processor):
        ITransformation(p, "rename entities"),
        rename_func(rename_func),
        rename_var(rename_var),
        processor(processor) {
}

unique_ptr<ASTConsumer> RenameEntitiesTransformation::getConsumer(Rewriter * rewriter) {
    return llvm::make_unique<RenameEntitiesASTConsumer>(rewriter, rename_func, rename_var, processor);
}

ITransformation * RenameEntitiesTransformation::buildFromConfig(const YAML::Node & config) {
    map<string, function<BaseRenameProcessor *(const YAML::Node &)>> renameProcessorFactory = {
            {"random", RandomRenameProcessor::buildFromConfig },
            {"test", TestRenameProcessor::buildFromConfig },
            {"hash", HashRenameProcessor::buildFromConfig }
    };

    const auto strategy = config["strategy"];
    const auto strategy_name = strategy["name"].as<string>();
    const auto processor = renameProcessorFactory[strategy_name](strategy);

    const auto p = config["p"].as<float>();
    const auto rename_func = config["rename functions"] != nullptr && config["rename functions"].as<bool>();
    const auto rename_var = config["rename variables"] != nullptr && config["rename variables"].as<bool>();
    return new RenameEntitiesTransformation(p, rename_func, rename_var, processor);
}
