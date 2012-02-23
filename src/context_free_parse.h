/******************************************************************************
 * This namespace performs context-free grammar analysis.
 * The tokenized source code is examined for the validity of
 *  its context-free grammar.
 *
 * More implementation details are placed in .cc file,
 *  but end users should have no need for those.
 ******************************************************************************/
#ifndef CONTEXT_FREE_PARSE_H
#define CONTEXT_FREE_PARSE_H

#include "common.h"
#include "parse_common.h"

namespace context_free_parse {

using namespace parse_common;   // Use parse_common here,
                                //  because it is like a parent namespace

// Get the number of states/symbols that need to be stepped back for reduction
unsigned getReductionSize(ProductionRule const& rule);

// Get common::Kind after reducing the production rule
common::Kind getReductionKind(ProductionRule const& rule);

// Convert non-terminal symbols in common::Kind into string.
std::string translateNonTerminal(common::Kind const& kind);

// LR1ParseRuleType determines which action the rule follows
// Reduce: Terminal or lower level non-terminal symbols are
//          reduced to a higher level non-terminal symbol
// Shift: Current symbol is shifted to the left.
enum LR1ParseRuleType {
    REDUCE,
    SHIFT,
};

// Rules for LR1 parsing
struct LR1ParseRule {
    LR1ParseRuleType type;
    unsigned state;
    common::Kind sym;   // Sym applies to non-terminal symbol when type = SHIFT
                        //  when type = REDUCE, only terminal symbol!
    unsigned next;      // Next applies to next state when type = SHIFT
                        //  when type = REDUCE, production rule!
};

class Tree {
    public:
        common::Token token;
        Tree *prev;
        Tree *next;
        Tree *down;

        Tree();
        ~Tree();

        void connect(Tree *rhs);    // this->next <---> rhs->prev
        void disconnect(Tree *rhs); // this->next <-/-> rhs->prev
};

class TreePtr {
    public:
        explicit TreePtr(Tree *tree);
        ~TreePtr();

        Tree* operator*();
    private:
        Tree *tree_;
};

// Build a tree structure for the source code.
Tree* build_parse_tree(common::Tokens const& tokens);

// Print out the tree structure in CFG form.
void print_parse_tree(Tree *root);

}

#endif//CONTEXT_FREE_PARSE_H
