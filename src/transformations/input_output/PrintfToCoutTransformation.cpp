#include "../../../include/transformations/input_output/PrintfToCoutTransformation.h"
#include "../../../include/transformations/input_output/Utils.h"

using clang::IfStmt, clang::CallExpr;
using clang::Lexer, clang::CharSourceRange, clang::SourceLocation;
using llvm::isa, llvm::cast, llvm::errs;
using std::stoi, std::vector, std::string, std::regex_search,
std::regex, std::sregex_iterator, std::distance;

// ------------ PrintfToCoutVisitor ------------

PrintfToCoutVisitor::PrintfToCoutVisitor(Rewriter * rewriter) :
        rewriter(rewriter), sm(rewriter->getSourceMgr()), opt(rewriter->getLangOpts()) {}

bool PrintfToCoutVisitor::VisitCallExpr(CallExpr *e) {
    auto srcloc = e->getBeginLoc();
    // Collecting printf or std::printf expressions
    if (sm.isWrittenInMainFile(srcloc)) {
        auto const srctext = getAsText(e);
        if (srctext == "std::printf" || srctext == "printf") {
            printfExpressions.push_back(e);
        }
    }
    return true;
}

bool PrintfToCoutVisitor::VisitUsingDirectiveDecl(UsingDirectiveDecl *ud) {
    auto srcloc = ud->getBeginLoc();
    // Collecting all used namespaces
    if (sm.isWrittenInMainFile(srcloc)) {
        usedNamespaces.push_back(ud);
    }
    return true;
}

bool PrintfToCoutVisitor::determineIfSimplePrintfCommand(const CallExpr * e) {
    string formatstring = getAsText(e->getArg(0)).str();

    // We rewrite simple printf commands directly.
    // If the number of args matches with the number from the regex,
    // we have a relative simple printf command
    regex const re(regexPlaceholders);

    int const match_count(distance(
            sregex_iterator(formatstring.begin(), formatstring.end(), re),
            sregex_iterator()));

    return (match_count == (e->getNumArgs() - 1));
}

void PrintfToCoutVisitor::insertHeaderAtBeginning(string headerstr) {
    headerstr = "#include <" + headerstr + ">\n";
    rewriter->InsertTextBefore(sm.getLocForStartOfFile(sm.getMainFileID()), headerstr);
}

StringRef PrintfToCoutVisitor::getAsText(const Expr * e) {
    SourceLocation LocStart = e->getBeginLoc();
    SourceLocation LocEnd = Lexer::getLocForEndOfToken(LocStart, 0, sm, opt);
    SourceRange range(LocStart, LocEnd);
    return Lexer::getSourceText(CharSourceRange::getCharRange(range), sm, opt);
}

string PrintfToCoutVisitor::parsePrecision(const vector<string> * matches) {
    if (matches->size() != 1) {
        errs() << "Format string has more then one match for \"%.Xf\"" << "\n";
        throw MatcherException();
    }
    auto match = matches->at(0);
    auto format_string = "%" + floatSplitRegex;
    vector<string> splits = split(&match, &format_string);
    return splits[1];
}

vector<string> * PrintfToCoutVisitor::getPrecisionPrefix(const vector<string> * specifiers) {
    auto prefix_vec = new vector<string>();

    for (const string &specifier : *specifiers) {
        string prefix;

        // Check if we have something like "%.10f"
        auto matches = getMatches(&specifier, &floatPrecisionRegex);
        if (specifier == "%e" || specifier == "%E") {
            prefix += _scientific + " << " +
                      _setprecision + "(" + default_precision + ")" + " << ";
        } else if (specifier == "%f" || !matches.empty()) {
            string precision = !matches.empty() ? parsePrecision(&matches) : default_precision;
            prefix += _fixed + " << " +
                      _setprecision + "(" + precision + ")" + " << ";
        }
        prefix_vec->push_back(prefix);
    }

    return prefix_vec;
}

bool PrintfToCoutVisitor::rewriteSimplePrintfCommand(const CallExpr *e) {
    // A. Definitions
    // Get the first printf argument = the format string with all the placeholders..
    string format = getAsText(e->getArg(0)).str();

    // B. Get all the substrings between the placeholders
    vector<string> splits = split(&format, &regexPlaceholders);

    // Removing quotes at the for first and last elem in splits
    if (splits.front().front() == '\"') {
        splits.front().erase(splits.front().begin());
    if (splits.back().back() == '\"')
        splits.back().pop_back();
    }

    if (splits.size() != e->getNumArgs()) {
        errs() << "The number of placeholders is not equal the number of arguments" << "\n";
        return false;
    }

    // C. get the placeholders
    vector<string> plholders = getMatches(&format, &regexPlaceholders);

    // D. Now get the correct precision for each substring depending on the respective placeholder
    vector<string> * prefixforeachplholder;
    try {
        prefixforeachplholder = getPrecisionPrefix(&plholders);
    } catch(MatcherException& ex) {
        errs() << "Precision-Prefix-Error during printf->cout rewrite" << "\n";
        return false;
    }

    // D. Construct the new cout command
    std::stringstream sstream;

    // Keep in mind: add std only if user has included the std namespace via "using namespace std"
    sstream << _cout;
    // Now build the rest
    for (unsigned i = 1; i < e->getNumArgs(); i++) {
        sstream << " << ";
        if (!splits[i - 1].empty()) {
            sstream << "\"" + splits[i - 1] + "\"" << " << ";
        }
        sstream << prefixforeachplholder->at(i - 1)
                << getAsText(e->getArg(i)).str();
    }
    if (!splits[e->getNumArgs() - 1].empty()) {
        sstream << " << ";
        sstream << "\"" + splits[e->getNumArgs() - 1] + "\"";
    }
    string coutCommand = sstream.str();

    // D. Replace the old printf command by the new cout command
    rewriter->ReplaceText(e->getSourceRange(), coutCommand);

    return true;
}

void PrintfToCoutVisitor::rewritePrintf() {
    for (auto e : printfExpressions) {
        if (determineIfSimplePrintfCommand(e)) {
            rewriteSimplePrintfCommand(e);
        }
    }
}

const char * MatcherException::what() const throw() {
    return "Error while transforming IO";
}

// ------------ PrintfToCoutASTConsumer ------------

PrintfToCoutASTConsumer::PrintfToCoutASTConsumer(Rewriter * rewriter) :
        visitor(rewriter) {}

void PrintfToCoutASTConsumer::HandleTranslationUnit(ASTContext &ctx) {
    visitor.TraverseDecl(ctx.getTranslationUnitDecl());
    visitor.rewritePrintf();
    visitor.insertHeaderAtBeginning("iostream");
    visitor.insertHeaderAtBeginning("iomanip");
}

// ------------ PrintfToCoutTransformation ------------

PrintfToCoutTransformation::PrintfToCoutTransformation(const float p) :
        ITransformation(p, "printf to cout") {}

unique_ptr<ASTConsumer> PrintfToCoutTransformation::getConsumer(Rewriter * rewriter) {
    return llvm::make_unique<PrintfToCoutASTConsumer>(rewriter);
}

ITransformation * PrintfToCoutTransformation::buildFromConfig(const YAML::Node & config) {
    const auto p = config["p"].as<float>();
    return new PrintfToCoutTransformation(p);
}
