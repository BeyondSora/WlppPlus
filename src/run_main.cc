#include "run_main.h"

#include <iostream>

#include "file.h"
#include "scan.h"

run_main::run_main() {}

run_main::~run_main() {}

void run_main::run()
{
    common::Words words = file::toWords("scan.h");

    std::cout << words.size() << "\n";
    std::cout << words[0].size() << "\n";
    for (unsigned i = 0; i < words.size(); ++i) {
        for (unsigned j = 0; j < words[i].size(); ++j) {
            std::cout << words[i][j] << " ";
        }
        std::cout << std::endl;
    }
    SCAN(words);
}
