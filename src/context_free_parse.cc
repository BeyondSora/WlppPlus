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

// Outputs wlppi format which would be used for building parse tree.
void outputWlppi(const common::Tokens &tokens);

// Parsing method similar to the one used in CS241 assignment 8
// Need to be revised in the future!!
Tree* build_parse_tree(const common::Tokens &tokens)
{


    Tree* tree = NULL;


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

void outputWlppi(const common::Tokens &tokens)
{
    std::vector<common::Token> src;
    tokensLinearize(src, tokens);   // Linearize tokens into a single line

    std::vector<common::Kind> stack;    // Symbol stack
    std::vector<int> states;            // LR1 states that correspond to the stack
    unsigned state = 0;     // LR1 state
    LR1ParseRule lr1;
    common::Kind sym;
    int symCounter = 0;
    bool cont = false;

    for (unsigned i = 0; i < src.size(); ++i) {
        symCounter++;
        sym = src[i].kind;

        cont = false;
        while (cont == false) {
            unsigned j = 0;
            for (; j < LR1_RULES_SIZE; ++j) {
                if (sym == LR1_RULES[j].sym && state == LR1_RULES[j].state) {
                    lr1 = LR1_RULES[j];
                    break;
                }
            }
            if (j == LR1_RULES_SIZE) {
                std::cerr << "ERROR at " << symCounter << std::endl;
                throw "Error in parsing\n";
            }

            switch (lr1.type) {
                case SHIFT:
                    stack.push_back(sym);
                    states.push_back(state);
                    cont = true;
                    break;
                case REDUCE:
                    unsigned rule = lr1.next;
                    //
                    //
                    break;
            }
        }
    }
}


}
