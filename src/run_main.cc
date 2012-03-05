/******************************************************************************
 * Copyright (C) 2012 Jimmy Lu
 ******************************************************************************/

#include "run_main.h"

#include <cstdlib>

#include "common.h"

#include "basic_io.h"
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

        basic_io::out("SUCCEEDED\n");

        for (unsigned i = 0; i < tokens.size(); ++i) {
            for (unsigned j = 0; j < tokens[i].size(); ++j) {
                basic_io::out(tokens[i][j].getKind() + ": " +
                              tokens[i][j].lexeme);
                if (tokens[i][j].kind == common::INT) {
                    basic_io::out(", number-form: " + tokens[i][j].toInt());
                }
                basic_io::out("\n");
            }
            basic_io::out("\n");
        }

        basic_io::out("----PARSE TREE: \n");
        context_free_parse::ParseTree parseTree(tokens);
        basic_io::out(parseTree.toString());
        semantic_parse::ParseTree semTree(parseTree.move());
    }
    catch (error::ErrorObjectInterface *e) {
        error::ErrorObjectPtr err(e);
        basic_io::out(err->toString() + "\n");
        std::exit(err->retCode());
    }
    catch (...) {
        basic_io::out("Encountered unknown error.\n");
        std::exit(1);
    }
    ///
    ///
    ///
}
