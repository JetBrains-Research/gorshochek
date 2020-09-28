#include "../../../include/transformations/io-transformations/IOUtils.h"

using std::smatch, std::vector, std::string, std::regex_search,
std::regex, std::sregex_iterator, std::distance, std::sregex_token_iterator;

vector<string> split(const string& input, const string& reg) {
    // passing -1 as the submatch index parameter performs splitting
    regex re(reg);
    sregex_token_iterator
            first{input.begin(), input.end(), re, -1},
            last;
    return {first, last};
}

vector<string> getMatches(string input, const string &re) {
    // Show matches
    regex simpleplholder(re);
    smatch res;
    vector<string> output;

    while (regex_search(input, res, simpleplholder)) {
        output.push_back(res[0].str());
        input = res.suffix();
    }
    return output;
}
