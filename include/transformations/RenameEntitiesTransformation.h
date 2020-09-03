#ifndef INCLUDE_TRANSFORMATIONS_RENAMEENTITIESTRANSFORMATION_H_
#define INCLUDE_TRANSFORMATIONS_RENAMEENTITIESTRANSFORMATION_H_


#include <string>
#include <vector>
#include <memory>
#include <map>
#include <random>

#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"

#include "../ITransformation.h"

using clang::ASTConsumer, clang::Rewriter, clang::RecursiveASTVisitor,
clang::ASTContext, clang::CallExpr, clang::Decl, clang::Stmt;
using std::unique_ptr, std::vector, std::map, std::string, std::mt19937,
std::discrete_distribution;

/// RecursiveASTVisitor is a set of actions that are done
/// when a certain node of AST is reached. RenameEntitiesVisitor in particular
/// visits all entities (e.g functions or variables) and rename them to a
/// random name
class RenameEntitiesVisitor : public RecursiveASTVisitor<RenameEntitiesVisitor> {
 public:
    explicit RenameEntitiesVisitor(Rewriter * rewriter, const bool rename_func, const bool rename_var,
                                   const int seed, const int max_tokens, const int max_token_len,
                                   const bool test = false);
    /**
     * This function is called a certain clang::CallExpr is visited. Here get all the functions
     * calls/declarations and rename them
     */
    bool VisitCallExpr(CallExpr * call);
    /**
     * This function is called a certain clang::Stmt is visited. Here get all the variables
     * calls/declarations and rename them
     */
    bool VisitStmt(Stmt * stmt);

 private:
    Rewriter * rewriter;

    const bool rename_func = false;
    const bool rename_var = false;

    const bool test = false;

    const int num_lat_chars = 26;
    discrete_distribution<int> token_len_generator;
    discrete_distribution<int> tokens_num_generator;
    discrete_distribution<int> char_generator;

    const map<string, string> testMapping = {
            {"do_math", "math_"},
            {"join", "jo"},
            {"result", "res"},
            {"val", "value"},
            {"count", "c"},
            {"S", "S1"},
            {"ffff", "f"},
            {"z", "zi"},
            {"x", "xi"},
            {"d", "doo"},
            {"omega", "om"},
            {"o", "oo"},
            {"i", "j"},
            {"totalset", "tt"}
    };

    mt19937 * gen;
    map<Decl *, string> decl2name;
    discrete_distribution<int> createUniformIntGenerator(const int num_elements);
    string randomSnakeCaseName();
};

class RenameEntitiesASTConsumer : public ASTConsumer {
 public:
    explicit RenameEntitiesASTConsumer(Rewriter * rewriter, const bool rename_func, const bool rename_var,
                                       const int seed, const int max_tokens, const int max_token_len,
                                       const bool test);
    void HandleTranslationUnit(ASTContext &ctx); // NOLINT.
 private:
    RenameEntitiesVisitor visitor;
};

class RenameEntitiesTransformation : public ITransformation {
 public:
    explicit RenameEntitiesTransformation(float p, const bool rename_func, const bool rename_var,
                                          const int seed, const int max_tokens, const int max_token_len,
                                          const bool test);
    ~RenameEntitiesTransformation();
    unique_ptr<ASTConsumer> getConsumer(Rewriter *rewriter);
 private:
    /**
     * Along with probability, the RenameEntitiesTransformation class constructor also accepts
     * entities which are going to renamed to random name
     */
    const bool rename_func = false;
    const bool rename_var = false;

    const int seed;
    const int max_tokens;
    const int max_token_len;

    const bool test = false;
};

#endif  // INCLUDE_TRANSFORMATIONS_RENAMEENTITIESTRANSFORMATION_H_
