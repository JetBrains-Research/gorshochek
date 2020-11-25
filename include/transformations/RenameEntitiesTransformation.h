#ifndef INCLUDE_TRANSFORMATIONS_RENAMEENTITIESTRANSFORMATION_H_
#define INCLUDE_TRANSFORMATIONS_RENAMEENTITIESTRANSFORMATION_H_

#include <yaml-cpp/yaml.h>

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

using clang::ASTConsumer, clang::Rewriter, clang::RecursiveASTVisitor, clang::FunctionDecl,
clang::ASTContext, clang::CallExpr, clang::Decl, clang::Stmt, clang::SourceManager;
using std::unique_ptr, std::vector, std::map, std::string, std::mt19937,
std::discrete_distribution;

/// RecursiveASTVisitor is a set of actions that are done
/// when a certain node of AST is reached. RenameEntitiesVisitor in particular
/// visits all entities (e.g functions or variables) and rename them to a
/// random name (or name like prefix_(hash from old name)
class RenameEntitiesVisitor : public RecursiveASTVisitor<RenameEntitiesVisitor> {
 public:
    explicit RenameEntitiesVisitor(Rewriter * rewriter, const bool rename_func, const bool rename_var,
                                   discrete_distribution<int> token_len_generator,
                                   discrete_distribution<int> tokens_num_generator,
                                   discrete_distribution<int> char_generator,
                                   mt19937 * gen, string  hash_prefix = "d",
                                   const bool with_hash = false,
                                   const bool test = false);
    /**
     * This function is called a certain clang::CallExpr is visited. Here get all the functions
     * calls/declarations and rename them
     */
    bool VisitCallExpr(CallExpr * call);
    /**
     * This function is called a certain clang::FunctionDecl is visited. Here get all the functions
     * declarations and rename them
     */
    bool VisitFunctionDecl(FunctionDecl * fdecl);
    /**
     * This function is called a certain clang::Stmt is visited. Here get all the variables
     * calls/declarations and rename them
     */
    bool VisitStmt(Stmt * stmt);

 private:
    Rewriter * rewriter;
    SourceManager & sm;

    const bool rename_func = false;
    const bool rename_var = false;

    discrete_distribution<int> token_len_generator;
    discrete_distribution<int> tokens_num_generator;
    discrete_distribution<int> char_generator;

    mt19937 * gen;

    const bool test = false;

    const bool with_hash = false;
    const string hash_prefix = "d";

    map<Decl *, string> decl2name;
    string randomSnakeCaseName();
    string hashName(string *name);
    void processVarDecl(Decl * decl, string * name);
};

class RenameEntitiesASTConsumer : public ASTConsumer {
 public:
    explicit RenameEntitiesASTConsumer(Rewriter * rewriter, const bool rename_func, const bool rename_var,
                                       discrete_distribution<int> token_len_generator,
                                       discrete_distribution<int> tokens_num_generator,
                                       discrete_distribution<int> char_generator,
                                       mt19937 * gen, const string& hash_prefix,
                                       const bool with_hash, const bool test);
    void HandleTranslationUnit(ASTContext &ctx); // NOLINT.
 private:
    RenameEntitiesVisitor visitor;
    Rewriter * rewriter;
};

class RenameEntitiesTransformation : public ITransformation {
 public:
    explicit RenameEntitiesTransformation(const float p, const bool rename_func,
                                          const bool rename_var, const int seed,
                                          const int max_tokens, const int max_token_len,
                                          const bool test);

    RenameEntitiesTransformation(const float p, const bool rename_func, const bool rename_var, const int seed,
                                 const int max_tokens, const int max_token_len, string hash_prefix,
                                 const bool with_hash, const bool test);

    ~RenameEntitiesTransformation() = default;
    unique_ptr<ASTConsumer> getConsumer(Rewriter *rewriter);
    static ITransformation * buildFromConfig(const YAML::Node & config);

 private:
    /**
     * Along with probability, the RenameEntitiesTransformation class constructor also accepts
     * entities which are going to renamed to random name
     */
    const bool rename_func = false;
    const bool rename_var = false;

    const bool test = false;

    const bool with_hash = false;
    const string hash_prefix = "d";

    const int num_lat_chars = 26;

    discrete_distribution<int> token_len_generator;
    discrete_distribution<int> tokens_num_generator;
    discrete_distribution<int> char_generator;

    mt19937 * gen;

    /**
     * This function creates discrete uniform distribution.
     * @param num_elements    the number of elements in the distribution
     * @return                discrete uniform distribution (i.e. with equal probabilities of each output)
     */
    static discrete_distribution<int> createUniformIntGenerator(const int num_elements);
};

#endif  // INCLUDE_TRANSFORMATIONS_RENAMEENTITIESTRANSFORMATION_H_
