#include "../../include/transformations/ForToWhileTransformation.h"

using clang::Expr, clang::ContinueStmt, clang::SourceRange;
using clang::Lexer, clang::CharSourceRange;
using llvm::isa, llvm::cast;
using std::unique_ptr, std::find, std::vector, std::string;

// ------------ ForToWhileVisitor ------------

ForToWhileVisitor::ForToWhileVisitor(Rewriter * rewriter) :
        rewriter(rewriter), sm(rewriter->getSourceMgr()), opt(rewriter->getLangOpts()) {}

bool ForToWhileVisitor::VisitForStmt(ForStmt * forStmt) {
    const Stmt * body = forStmt->getBody();

    if (!body) {
        return true;
    }

    processInit(forStmt);
    processCond(forStmt);
    processInc(forStmt);
    processBody(forStmt);

    return true;
}

void ForToWhileVisitor::processBody(ForStmt *forStmt) {
    const Stmt * body = forStmt->getBody();
    // If the inc part of for is empty, then it is not necessary to add brackets
    if (forStmt->getInc()) {
        if (!isa<clang::CompoundStmt>(body)) {
            rewriter->InsertText(body->getBeginLoc(), "{\n", true, true);
            rewriter->InsertText(body->getEndLoc().getLocWithOffset(1), "}", true, true);
        }
    }
}

void ForToWhileVisitor::processInit(ForStmt * forStmt) {
    Stmt * init = forStmt->getInit();
    if (init) {
        SourceRange initRange = init->getSourceRange();
        if (isa<clang::ValueStmt>(*init)) {
            initRange.setEnd(init->getEndLoc().getLocWithOffset(1));
        }
        auto initText = Lexer::getSourceText(CharSourceRange::getCharRange(initRange), sm, opt).str() + ";\n";
        rewriter->InsertText(forStmt->getBeginLoc(), initText, true, true);
    }
}

void ForToWhileVisitor::processCond(ForStmt * forStmt) {
    Expr * cond = forStmt->getCond();
    string condText;
    if (cond) {
        SourceRange condRange = cond->getSourceRange();
        if (isa<clang::ValueStmt>(*cond->getExprStmt())) {
            condRange.setEnd(cond->getEndLoc().getLocWithOffset(1));
        }
        condText = Lexer::getSourceText(CharSourceRange::getCharRange(condRange), sm, opt).str();
    } else {
        condText = "1";
    }
    auto whileText = "while (" + condText + ") ";
    ptrdiff_t diff = sm.getCharacterData(forStmt->getBody()->getBeginLoc()) -
                      sm.getCharacterData(forStmt->getBeginLoc());
    rewriter->ReplaceText(forStmt->getBeginLoc(),
                          static_cast<unsigned int>(diff), whileText);
}

void ForToWhileVisitor::processInc(ForStmt * forStmt) {
    Expr * inc = forStmt->getInc();
    if (!inc) {
        return;
    } else {
        SourceRange incRange = inc->getSourceRange();
        if (isa<clang::ValueStmt>(*inc->getExprStmt())) {
            if (isa<clang::UnaryOperator>(inc) && cast<clang::UnaryOperator>(inc)->isPostfix())
                incRange.setEnd(inc->getEndLoc().getLocWithOffset(2));
            else
                incRange.setEnd(inc->getEndLoc().getLocWithOffset(1));
        }
        auto incText = Lexer::getSourceText(CharSourceRange::getCharRange(incRange), sm, opt).str() + "; ";
        vector<const ContinueStmt *> continues;
        collectContinues(forStmt->getBody(), &continues);
        for (auto countinue_stmt : continues) {
            rewriter->InsertText(countinue_stmt->getBeginLoc(), incText, true, true);
        }
        incText += "\n";
        const Stmt * body = forStmt->getBody();
        clang::SourceLocation forEndLoc;
        if (!isa<clang::CompoundStmt>(body)) {
            forEndLoc = forStmt->getEndLoc().getLocWithOffset(1);
            incText = "\n" + incText;
        } else {
            forEndLoc = forStmt->getEndLoc();
        }
        rewriter->InsertText(forEndLoc, "\t" + incText, true, true);
    }
}

void ForToWhileVisitor::collectContinues(const Stmt * s, vector<const ContinueStmt *> *continues) {
    if (const ContinueStmt * dec = clang::dyn_cast<ContinueStmt>(s)) {
        continues->push_back(dec);
    }
    const auto &children = s->children();
    for (const clang::Stmt *child : children) {
        if (child) {
            collectContinues(child, continues);
        }
    }
}

// ------------ ForToWhileASTConsumer ------------

ForToWhileASTConsumer::ForToWhileASTConsumer(Rewriter * rewriter) :
        visitor(rewriter) {}

bool ForToWhileASTConsumer::HandleTopLevelDecl(DeclGroupRef DR) {
    for (clang::Decl * b : DR) {
        // Traverse each declaration in DeclGroup using our AST visitor.
        visitor.TraverseDecl(b);
        b->dump();
    }
    return true;
}

// ------------ ForToWhileTransformation ------------

ForToWhileTransformation::ForToWhileTransformation(const float p) :
        ITransformation(p, "for to while") {}

unique_ptr<ASTConsumer> ForToWhileTransformation::getConsumer(Rewriter * rewriter) {
    return llvm::make_unique<ForToWhileASTConsumer>(rewriter);
}

ITransformation * ForToWhileTransformation::buildFromConfig(const YAML::Node & config) {
    const auto p = config["p"].as<float>();
    return new ForToWhileTransformation(p);
}