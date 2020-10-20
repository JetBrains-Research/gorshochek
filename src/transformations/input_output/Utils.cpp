#include "../../../include/transformations/input_output/Utils.h"

using std::smatch, std::vector, std::string, std::regex_search,
std::regex, std::sregex_iterator, std::distance, std::sregex_token_iterator;

void split(const string * input, const string * reg, vector<string> * splits) {
    // passing -1 as the submatch index parameter performs splitting
    regex re(*reg);
    sregex_token_iterator
            first{input->begin(), input->end(), re, -1},
            last;
    splits->assign(first, last);
}

void getMatches(const string * input, const string * re, vector<string> * matches) {
    // Show matches
    regex simpleplholder(*re);
    smatch res;
    string input_str = *input;

    while (regex_search(input_str, res, simpleplholder)) {
        matches->push_back(res[0].str());
        input_str = res.suffix();
    }
}
