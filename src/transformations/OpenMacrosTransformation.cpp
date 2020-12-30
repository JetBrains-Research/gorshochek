#include "../../include/transformations/OpenMacrosTransformation.h"

#include <utility>
#include <iostream>

using llvm::isa, llvm::cast;
using std::unique_ptr, std::find, std::vector, std::string, std::to_string, std::hash, std::function;
using clang::Lexer, clang::CharSourceRange, clang::PrintingPolicy;
using llvm::raw_string_ostream;

// ------------ OpenMacrosVisitor ------------

OpenMacrosVisitor::OpenMacrosVisitor(Rewriter * rewriter):
        rewriter(rewriter), sm(rewriter->getSourceMgr()), opt(rewriter->getLangOpts()) {
}

bool OpenMacrosVisitor::VisitFunctionDecl(FunctionDecl * func) {
    auto loc = func->getBeginLoc();
    if (!sm.isWrittenInMainFile(loc)) {
        return true;
    }
    string opened_macros;
    raw_string_ostream stream(opened_macros);
    PrintingPolicy pp(opt);
    pp.adjustForCPlusPlus();
    func->print(stream, pp);
    stream.flush();
    auto expansionRange = Lexer::getAsCharRange(func->getSourceRange(), sm, opt);
    if (loc.isMacroID()) {
        expansionRange = sm.getImmediateExpansionRange(loc);
        loc = expansionRange.getBegin();
    }

    auto old_macros = Lexer::getSourceText(expansionRange, sm, opt).str();
    rewriter->ReplaceText(loc, old_macros.length(), opened_macros);
    return true;
}

// ------------ OpenMacrosASTConsumer ------------

OpenMacrosASTConsumer::OpenMacrosASTConsumer(Rewriter * rewriter):
        visitor(rewriter),
        rewriter(rewriter) {}

void OpenMacrosASTConsumer::HandleTranslationUnit(ASTContext &ctx) {
    visitor.TraverseDecl(ctx.getTranslationUnitDecl());
    rewriter->overwriteChangedFiles();
}

// ------------ OpenMacrosTransformation ------------

OpenMacrosTransformation::OpenMacrosTransformation(const float p):
        ITransformation(p, "open macros") {}

unique_ptr<ASTConsumer> OpenMacrosTransformation::getConsumer(Rewriter * rewriter) {
    return llvm::make_unique<OpenMacrosASTConsumer>(rewriter);
}

ITransformation * OpenMacrosTransformation::buildFromConfig(const YAML::Node & config) {
    const auto p = config["p"].as<float>();
    return new OpenMacrosTransformation(p);
}
