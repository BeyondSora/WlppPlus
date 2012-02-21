#include "context_free_parse.h"

#include <iostream>

#include "lr1_rules.h"

namespace context_free_parse {

using namespace lr1_rules;  // LR1_RULES is only in another file
                            //  because of its large number of code.

// Turn Tokens into one single line of tokenized input,
//  and also prepends BOF and EOF to the source code.
// Need some rework in the future to improve efficiency.
void tokensLinearize(std::vector<common::Token> &src,
                     const common::Tokens &tokens);

// Parsing method similar to the one I used in CS241 assignment 8.
// Need to be revised in the future!!
Tree* build_parse_tree(const common::Tokens &tokens)
{
    Tree *tree = NULL;



    return tree;
}

void tokensLinearize(std::vector<common::Token> &src,
                     const common::Tokens &tokens)
{
    common::Token bof = { common::bof, "bof" };
    src.push_back(bof);

    for (unsigned i = 0; i < tokens.size(); ++i)
        for (unsigned j = 0; j < tokens[i].size(); ++j)
            src.push_back(tokens[i][j]);

    common::Token eof = { common::eof, "eof" };
    src.push_back(bof);
}

}
