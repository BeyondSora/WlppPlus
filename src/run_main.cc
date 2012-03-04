/******************************************************************************
 * Copyright (C) 2012 Jimmy Lu
 ******************************************************************************/

#include "run_main.h"

#include <cstdlib>
#include <iostream>

#include "common.h"

#include "context_free_parse.h"
#include "file.h"
#include "scan.h"
#include "semantic_parse.h"

#include "error.h"

run_main::run_main(int argc, char **argv)
{
    for (int i = 0; i < argc; ++i) {
        flags_.push_back(argv[i]);
    }
}

run_main::~run_main() {}

void run_main::run()
{
    // Code for testing!!
    //
    //
    try {
        common::Lines lines = file::toLines(flags_.at(1));

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

        std::cout << "----PARSE TREE: \n";
        context_free_parse::ParseTree parseTree(tokens);
        std::cout << parseTree.toString();
        semantic_parse::ParseTree semTree(parseTree.move());
    }
    catch (error::ErrorObjectInterface *e) {
        error::ErrorObjectPtr err(e);
        std::cout << err->toString() << std::endl;
        std::exit(err->retCode());
    }
    catch (...) {
        std::cout << "Encountered unknown error." << std::endl;
        std::exit(1);
    }
    ///
    ///
    ///
}
