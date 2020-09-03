#include "../../include/transformations/RenameEntitiesTransformation.h"

using llvm::isa, llvm::cast;
using std::unique_ptr, std::find, std::vector, std::string;
using clang::FunctionDecl, clang::VarDecl, clang::DeclRefExpr;

// ------------ RenameEntitiesVisitor ------------

RenameEntitiesVisitor::RenameEntitiesVisitor(Rewriter * rewriter, const vector<string> * entities,
                                             const int seed) :
        rewriter(rewriter), entities(entities) {
    gen = new mt19937(seed);
    token_len_generator = createUniformIntGenerator(max_token_len);
    tokens_num_generator = createUniformIntGenerator(max_tokens);
    char_generator = createUniformIntGenerator(num_lat_chars);
}

discrete_distribution<int> RenameEntitiesVisitor::createUniformIntGenerator(const int num_elements) {
    vector<double> weights;
    for (int i = 0; i < num_elements; ++i) {
        weights.push_back(100 / num_elements);
    }
    return discrete_distribution<int> (weights.begin(), weights.end());
}

bool RenameEntitiesVisitor::VisitStmt(Stmt * stmt) {
    if (containsEntity("variables")) {
        if (isa<DeclRefExpr>(stmt)) {
            auto * de = cast<DeclRefExpr>(stmt);
            auto * decl = de->getDecl();
            if (isa<VarDecl>(decl)) {
                string name = de->getNameInfo().getAsString();
                if (decl2name.find(decl) == decl2name.end()) {
                    string randomName = randomSnakeCaseName();
                    rewriter->ReplaceText(decl->getLocation(), name.length(), randomName);
                    decl2name[decl] = randomName;
                }
                rewriter->ReplaceText(de->getEndLoc(), name.length(), decl2name[decl]);
            }
        }
    }
    return true;
}

bool RenameEntitiesVisitor::VisitCallExpr(CallExpr * call) {
    if (containsEntity("functions") && call->getDirectCallee()) {
        FunctionDecl * f  = call->getDirectCallee();
        string name = f->getNameInfo().getName().getAsString();
        if (decl2name.find(f) == decl2name.end()) {
            string randomName = randomSnakeCaseName();
            rewriter->ReplaceText(f->getLocation(), name.length(), randomName);
            decl2name[f] = randomName;
        }
        rewriter->ReplaceText(call->getExprLoc(), name.length(), decl2name[f]);
    }
    return true;
}

bool RenameEntitiesVisitor::containsEntity(string entity) {
    return find(entities->begin(), entities->end(), entity) != entities->end();
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

RenameEntitiesASTConsumer::RenameEntitiesASTConsumer(Rewriter * rewriter, const vector<string> * entities,
                                                     const int seed):
        visitor(rewriter, entities, seed) {}

void RenameEntitiesASTConsumer::HandleTranslationUnit(ASTContext &ctx) {
    visitor.TraverseDecl(ctx.getTranslationUnitDecl());
}

// ------------ RenameEntitiesTransformation ------------

RenameEntitiesTransformation::RenameEntitiesTransformation(float p, const vector<string> * entities,
                                                           const int seed) :
        ITransformation(p, "rename entities"),
        entities(entities),
        seed(seed) {}

RenameEntitiesTransformation::~RenameEntitiesTransformation() {}

unique_ptr<ASTConsumer> RenameEntitiesTransformation::getConsumer(Rewriter * rewriter) {
    return llvm::make_unique<RenameEntitiesASTConsumer>(rewriter, entities, seed);
}
