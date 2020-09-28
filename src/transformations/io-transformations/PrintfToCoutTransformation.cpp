#include "../../../include/transformations/io-transformations/PrintfToCoutTransformation.h"
#include "../../../include/transformations/io-transformations/IOUtils.h"

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
            printfexpressions.push_back(e);
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

string PrintfToCoutVisitor::append_std(string word) {
    for (auto ud : usedNamespaces) {
        if (ud->getNominatedNamespace()->getQualifiedNameAsString() == word) {
            return word;
        }
    }
    return "std::"+ word;
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

vector<string> PrintfToCoutVisitor::getPrecisionPrefix(const vector<string> &plholdersvec) {
    vector<string> prefixlist;

    string cur_mode;  // we only need to add fixed if fixed changed to scientific or vice versa
    int cur_precision = -1;  // we only need to add setprecision if precision changed..


    for (const string &plholder : plholdersvec) {
        string cur_prefix;

        // Check if we have s.th. like "%.10f"
        auto floatmatches = getMatches(plholder, floatPrecisionRegex);
        if (!floatmatches.empty()) {
            if (floatmatches.size() != 1) {
                errs() << "Warning: Format string has != 1 matches for %.Xf." << "\n";
                throw IOTransformationException();
            }
            vector<string> splits = split(floatmatches[0], floatSplitRegex);
            if (splits.size() != 2) {
                errs() << "Error: Splits != 2.; size:"
                       << splits.size() << "for:"
                       << floatmatches[0] << "\n";
                throw IOTransformationException();
            } else {
                if (cur_mode != "fixed") {
                    cur_prefix += append_std("fixed");
                    cur_mode = "fixed";
                }
                if (std::stoi(splits[1]) != cur_precision) {
                    cur_precision = stoi(splits[1]);
                    cur_prefix += " << " + append_std("setprecision(" + splits[1] + ") << ");
                }
            }
        } else if (plholder == "%f") {
            // Check if only %f, %e, %E,
            // printf's default precision is 6.. that's why we use
            // setprecision(6) in the default case %f
            if (cur_mode != "fixed") {
                cur_prefix += append_std("fixed") + " << ";
                cur_mode = "fixed";
            }
            if (cur_precision != 6) {
                cur_precision = 6;
                cur_prefix += append_std("setprecision(6)") + " << ";
            }

        } else if (plholder == "%e" || plholder == "%E") {
            if (cur_mode != "scientific") {
                cur_prefix += append_std("scientific") + " << ";
                cur_mode = "scientific";
            }
            if (cur_precision != 6) {
                cur_precision = 6;
                cur_prefix += append_std("setprecision(6)") + " << ";
            }
        }
        prefixlist.push_back(cur_prefix);
    }

    if (prefixlist.size() != plholdersvec.size()) {
        errs() << "placeholders and prefix list mismatch" << "\n";
        throw IOTransformationException();
    }

    return prefixlist;
}

bool PrintfToCoutVisitor::rewriteSimplePrintfCommand(const CallExpr *e) {
    // A. Definitions
    // Get the first printf argument = the format string with all the placeholders..
    string formatstring = getAsText(e->getArg(0)).str();
    // B. Get all the substrings between the placeholders
    vector<string> splits = split(formatstring, regexPlaceholders);

    // Assure the number of splits equals the number of arguments
    if (splits.size() != e->getNumArgs()) {
        errs() << "Splits != Num Args" << "\n";
        return false;
    }

    // C. get the placeholders
    vector<string> plholders = getMatches(formatstring, regexPlaceholders);

    if (splits.size() - 1 != plholders.size()) {
        errs() << splits.size() << ";"
               << plholders.size() << "Splits-1 != plholders.size()"
               << "\n";
        return false;
    }

    // D. Now get the correct precision for each substring depending on the respective placeholder
    std::vector<std::string> prefixforeachplholder;
    try {
        prefixforeachplholder = getPrecisionPrefix(plholders);
    } catch(IOTransformationException& ex) {
        errs() << "Precision-Prefix-Error during printf->cout rewrite:" << "\n";
        return false;
    }

    // D. Construct the new cout command
    std::stringstream sstream;

    // Keep in mind: add std only if user has included the std namespace via "using namespace std"
    sstream << "" << append_std("cout");
    // Now build the rest
    for (unsigned i = 1; i < e->getNumArgs(); i++) {
        sstream << " << " << prefixforeachplholder[i - 1] << getAsText(e->getArg(i)).str();
    }
    string coutCommand = sstream.str();

    // D. Replace the old printf command by the new cout command
    rewriter->ReplaceText(e->getSourceRange(), coutCommand);

    return true;
}

void PrintfToCoutVisitor::rewritePrintf() {
    for (auto e : printfexpressions) {
        if (determineIfSimplePrintfCommand(e)) {
            rewriteSimplePrintfCommand(e);
        }
    }
}

const char * IOTransformationException::what() const throw() {
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
