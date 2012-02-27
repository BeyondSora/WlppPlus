/******************************************************************************
 * This namespace performs context-free grammar analysis.
 * The tokenized source code is examined for the validity of
 *  its context-free grammar.
 *
 * More implementation details are placed in source file,
 *  but end users should have no need for those.
 ******************************************************************************/
#ifndef CONTEXT_FREE_PARSE_H
#define CONTEXT_FREE_PARSE_H

#include "common.h"
#include "parse_common.h"

namespace context_free_parse {

using namespace parse_common;   // Use parse_common here,
                                //  because it is like a parent namespace

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
    common::Kind sym;   // Sym applies to non-terminal symbol when type == SHIFT
                        //  when type == REDUCE, only terminal symbol!
    unsigned next;      // Next applies to next state when type == SHIFT
                        //  when type == REDUCE, production rule!
};

//class Tree;     // Base unit for the ParseTree.
                // Implementation details are placed in source file.

class ParseTree {
    public:
        explicit ParseTree(common::Tokens const& tokens);
        ~ParseTree();

        Tree* operator*();  // Returns tree_.
        std::string toString();
        Tree* move();   // Returns tree_ and then nullify it.
    private:
        Tree *tree_;

        // Build a tree structure for the source code.
        static Tree* build_parse_tree(common::Tokens const& tokens);
        // Convert a tree structure into string in CFG format.
        static void convTreeToString(Tree *root, std::string &str);
};




}

#endif//CONTEXT_FREE_PARSE_H
