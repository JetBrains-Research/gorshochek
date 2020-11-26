#include "../../include/transformations/RenameEntitiesTransformation.h"

#include <utility>

using llvm::isa, llvm::cast;
using std::unique_ptr, std::find, std::vector, std::string, std::to_string, std::hash;
using clang::DeclStmt, clang::FunctionDecl, clang::VarDecl, clang::DeclRefExpr,
clang::NamedDecl;

// ------------ RenameEntitiesVisitor ------------

RenameEntitiesVisitor::RenameEntitiesVisitor(Rewriter * rewriter, const bool rename_func, const bool rename_var,
                                             discrete_distribution<int> token_len_generator,
                                             discrete_distribution<int> tokens_num_generator,
                                             discrete_distribution<int> char_generator,
                                             mt19937 * gen, const string * hash_prefix, const bool with_hash,
                                             const bool test):
        rewriter(rewriter), sm(rewriter->getSourceMgr()),
        rename_func(rename_func), rename_var(rename_var),
        token_len_generator(move(token_len_generator)),
        tokens_num_generator(move(tokens_num_generator)),
        char_generator(move(char_generator)),
        gen(gen), hash_prefix(*hash_prefix), with_hash(with_hash), test(test)  {
}

bool RenameEntitiesVisitor::VisitFunctionDecl(FunctionDecl * fdecl) {
    if (rename_func) {
        auto loc = fdecl->getBeginLoc();
        if (sm.isWrittenInMainFile(loc) && !(fdecl->isMain())) {
            string name = fdecl->getNameInfo().getName().getAsString();
            if (decl2name.find(fdecl) == decl2name.end()) {
                processVarDecl(fdecl, &name);
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
                rewriter->ReplaceText(de->getExprLoc(), name.length(), decl2name.at(decl));
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
        if (sm.isWrittenInMainFile(loc)) {
            string name = fdecl->getNameInfo().getName().getAsString();
            if (decl2name.find(fdecl) == decl2name.end()) {
                processVarDecl(fdecl, &name);
            }
            rewriter->ReplaceText(call->getExprLoc(), name.length(), decl2name.at(fdecl));
        }
    }
    return true;
}

string RenameEntitiesVisitor::randomSnakeCaseName() {
    string newName;
    int num_tokens = tokens_num_generator(*gen) + 1;
    for (int tok_idx = 0; tok_idx < num_tokens; ++tok_idx) {
        int token_len = token_len_generator(*gen) + 1;
        for (int ch_idx = 0; ch_idx < token_len; ++ch_idx) {
            char newChar = 'a' + char_generator(*gen);
            newName += newChar;
        }
        if (tok_idx != num_tokens - 1) {
            newName += "_";
        }
    }
    return newName;
}

string RenameEntitiesVisitor::hashName(string * name) {
    return hash_prefix + "_" + to_string(hash<string>{}(*name));
}

void RenameEntitiesVisitor::processVarDecl(Decl * decl, string * name) {
    string randomName;
    if (test) {
        randomName = "test_" + *name;
    } else {
        randomName = with_hash ? hashName(name) : randomSnakeCaseName();
    }
    rewriter->ReplaceText(decl->getLocation(), name->length(), randomName);
    decl2name[decl] = randomName;
}

// ------------ RenameEntitiesASTConsumer ------------

RenameEntitiesASTConsumer::RenameEntitiesASTConsumer(Rewriter * rewriter, const bool rename_func, const bool rename_var,
                                                     discrete_distribution<int> token_len_generator,
                                                     discrete_distribution<int> tokens_num_generator,
                                                     discrete_distribution<int> char_generator,
                                                     mt19937 * gen, const string * hash_prefix,
                                                     const bool with_hash, const bool test):
        visitor(rewriter, rename_func, rename_var,
                move(token_len_generator), move(tokens_num_generator),
                move(char_generator), gen, hash_prefix, with_hash, test),
        rewriter(rewriter) {}

void RenameEntitiesASTConsumer::HandleTranslationUnit(ASTContext &ctx) {
    visitor.TraverseDecl(ctx.getTranslationUnitDecl());
    rewriter->overwriteChangedFiles();
}

// ------------ RenameEntitiesTransformation ------------

RenameEntitiesTransformation::RenameEntitiesTransformation(const float p, const bool rename_func,
                                                           const bool rename_var, const int seed,
                                                           const int max_tokens, const int max_token_len,
                                                           const string * hash_prefix,
                                                           const bool with_hash, const bool test):
        ITransformation(p, "rename entities"),
        rename_func(rename_func),
        rename_var(rename_var),
        hash_prefix(*hash_prefix),
        with_hash(with_hash),
        test(test) {
    gen = new mt19937(seed);
    token_len_generator = createUniformIntGenerator(max_token_len);
    tokens_num_generator = createUniformIntGenerator(max_tokens);
    char_generator = createUniformIntGenerator(num_lat_chars);
}


discrete_distribution<int> RenameEntitiesTransformation::createUniformIntGenerator(const int num_elements) {
    /**
     * Such a strange way to generate random variables is essential to guarantee
     * the same behaviour of random module on different OS.
     * This way of generated random values is suggested in the following post:
     * http://anadoxin.org/blog/c-shooting-yourself-in-the-foot-4.html
     */
    auto probabilities = vector<double>(num_elements);
    for (int i = 0; i < num_elements; ++i) {
        // Here we create a discrete uniform distribution with probability
        // equals to 100 / num_elements
        probabilities[i] = 100.0 / num_elements;
    }
    return discrete_distribution<int> (probabilities.begin(), probabilities.end());
}

unique_ptr<ASTConsumer> RenameEntitiesTransformation::getConsumer(Rewriter * rewriter) {
    return llvm::make_unique<RenameEntitiesASTConsumer>(rewriter, rename_func, rename_var,
                                                        token_len_generator, tokens_num_generator,
                                                        char_generator, gen, &hash_prefix, with_hash, test);
}

ITransformation * RenameEntitiesTransformation::buildFromConfig(const YAML::Node & config) {
    const auto p = config["p"].as<float>();
    const auto rename_func = config["rename functions"] != nullptr && config["rename functions"].as<bool>();
    const auto rename_var = config["rename variables"] != nullptr && config["rename variables"].as<bool>();
    const auto test = config["test"] != nullptr && config["test"].as<bool>();
    const auto seed = config["seed"].as<int>();
    const auto max_tokens = config["max tokens"].as<int>();
    const auto max_token_len = config["max token len"].as<int>();
    const auto hash_prefix = config["hash prefix"] != nullptr ? config["hash prefix"].as<string>() : "d";
    const auto with_hash = (config["with hash"] != nullptr) && config["with hash"].as<bool>();
    return new RenameEntitiesTransformation(p, rename_func, rename_var, seed,
                                            max_tokens, max_token_len, &hash_prefix, with_hash, test);
}
