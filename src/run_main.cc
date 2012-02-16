#include "run_main.h"

#include <iostream>

#include "common.h"

#include "file.h"
#include "scan.h"

run_main::run_main() {}

run_main::~run_main() {}

void run_main::run()
{
    // Code for testing!!
    //
    //

    common::Lines lines = file::toLines("test.wlpp");

    for (unsigned i = 0; i < lines.size(); ++i) {
        //std::cout << lines[i] << std::endl;
    }

    common::Tokens tokens = scan::tokenize(lines);

    std::cout << "SUCCEEDED" << std::endl;

    for (unsigned i = 0; i < tokens.size(); ++i) {
        for (unsigned j = 0; j < tokens[i].size(); ++j) {
            //std::cout << tokens[i][j].lexeme << " ";
            std::cout << tokens[i][j].getKind() << ": " <<
                         tokens[i][j].lexeme;
            if (tokens[i][j].kind == common::INT)
                std::cout << ", number-form: " << tokens[i][j].toInt();
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}
