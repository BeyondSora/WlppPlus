#include "run_main.h"

#include <iostream>

#include "file.h"
#include "scan.h"

run_main::run_main() {}

run_main::~run_main() {}

void run_main::run()
{
    common::Lines lines = file::toLines("common.h");

    for (unsigned i = 0; i < lines.size(); ++i) {
        std::cout << lines[i] << std::endl;
    }

    common::Tokens tokens = SCAN(lines);
}
