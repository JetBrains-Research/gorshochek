#include <iostream>

#include "include/Utils.h"


using std::cerr, std::endl;

int main(int argc, const char **argv) {
    if (argc < 2)
        cerr << "Not enough positional command line"
             << "arguments specified" << endl;
    Runner(argc, argv).run();
    return 0;
}
