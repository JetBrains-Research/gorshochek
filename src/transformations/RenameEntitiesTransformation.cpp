#include "../../include/transformations/RenameEntitiesTransformation.h"

using llvm::isa, llvm::cast;
using std::unique_ptr, std::find, std::vector, std::string;
using clang::FunctionDecl, clang::VarDecl, clang::DeclRefExpr;

// ------------ RenameEntitiesVisitor ------------

RenameEntitiesVisitor::RenameEntitiesVisitor(Rewriter * rewriter, const bool rename_func, const bool rename_var,
                                             discrete_distribution<int> token_len_generator,
                                             discrete_distribution<int> tokens_num_generator,
                                             discrete_distribution<int> char_generator,
                                             mt19937 * gen, const bool test):
        rewriter(rewriter), rename_func(rename_func), rename_var(rename_var),
        token_len_generator(token_len_generator),
        tokens_num_generator(tokens_num_generator),
        char_generator(char_generator),
        gen(gen), test(test) {
}

bool RenameEntitiesVisitor::VisitStmt(Stmt * stmt) {
    if (rename_var) {
        if (isa<DeclRefExpr>(stmt)) {
            auto * de = cast<DeclRefExpr>(stmt);
            auto * decl = de->getDecl();
            if (isa<VarDecl>(decl)) {
                string name = de->getNameInfo().getAsString();
                if (decl2name.find(decl) == decl2name.end()) {
                    string randomName = test ? "test_" + name : randomSnakeCaseName();
                    rewriter->ReplaceText(decl->getLocation(), name.length(), randomName);
                    decl2name[decl] = randomName;
                }
                rewriter->ReplaceText(de->getExprLoc(), name.length(), decl2name.at(decl));
            }
        }
    }
    return true;
}

bool RenameEntitiesVisitor::VisitCallExpr(CallExpr * call) {
    if (rename_func && call->getDirectCallee()) {
        FunctionDecl * f  = call->getDirectCallee();
        string name = f->getNameInfo().getName().getAsString();
        if (decl2name.find(f) == decl2name.end()) {
            string randomName = test ? "test_" + name : randomSnakeCaseName();
            rewriter->ReplaceText(f->getLocation(), name.length(), randomName);
            decl2name[f] = randomName;
        }
        rewriter->ReplaceText(call->getExprLoc(), name.length(), decl2name.at(f));
    }
    return true;
}

string RenameEntitiesVisitor::randomSnakeCaseName() {
    string newName = "";
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

// ------------ RenameEntitiesASTConsumer ------------

RenameEntitiesASTConsumer::RenameEntitiesASTConsumer(Rewriter * rewriter, const bool rename_func, const bool rename_var,
                                                     discrete_distribution<int> token_len_generator,
                                                     discrete_distribution<int> tokens_num_generator,
                                                     discrete_distribution<int> char_generator,
                                                     mt19937 * gen, const bool test):
        visitor(rewriter, rename_func, rename_var,
                token_len_generator, tokens_num_generator,
                char_generator, gen, test) {}

void RenameEntitiesASTConsumer::HandleTranslationUnit(ASTContext &ctx) {
    visitor.TraverseDecl(ctx.getTranslationUnitDecl());
}

// ------------ RenameEntitiesTransformation ------------

RenameEntitiesTransformation::RenameEntitiesTransformation(float p, const bool rename_func, const bool rename_var,
                                                           const int seed, const int max_tokens,
                                                           const int max_token_len, const bool test):
        ITransformation(p, "rename entities"),
        rename_func(rename_func),
        rename_var(rename_var),
        test(test) {
    gen = new mt19937(seed);
    token_len_generator = createUniformIntGenerator(max_token_len);
    tokens_num_generator = createUniformIntGenerator(max_tokens);
    char_generator = createUniformIntGenerator(num_lat_chars);
}


discrete_distribution<int> RenameEntitiesTransformation::createUniformIntGenerator(const int num_elements) {
    vector<double> probabilities;
    for (int i = 0; i < num_elements; ++i) {
        // Here we create a discrete uniform distribution with probability
        // equals to 100 / num_elements
        probabilities.push_back(100 / num_elements);
    }
    return discrete_distribution<int> (probabilities.begin(), probabilities.end());
}

RenameEntitiesTransformation::~RenameEntitiesTransformation() {}

unique_ptr<ASTConsumer> RenameEntitiesTransformation::getConsumer(Rewriter * rewriter) {
    return llvm::make_unique<RenameEntitiesASTConsumer>(rewriter, rename_func, rename_var,
                                                        token_len_generator, tokens_num_generator,
                                                        char_generator, gen, test);
}
