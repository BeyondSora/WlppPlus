/******************************************************************************
 * Copyright (C) 2012 Jimmy Lu
 *
 * This namespace performs semantic analysis.
 * Various forms of type-check's are performed.
 *
 * More implementation details are placed in source file,
 *  but end users should have no need for those.
 ******************************************************************************/
#ifndef SEMANTIC_PARSE_H
#define SEMANTIC_PARSE_H

#include "common.h"

#include <map>

#include "parse_common.h"

namespace semantic_parse {

using namespace parse_common;   // Use parse_common here,
                                //  because it is like a parent namespace.

struct VectorTree {
    ProductionRule rule;
    common::Token token;
    std::vector<VectorTree> leaves;
};

class ParseTree: public ParseTreeInterface {
    typedef std::map<std::string, std::string> SymbolTable;
    public:
        explicit ParseTree(Tree* tree);
    private:
        VectorTree vecTree_;
        SymbolTable symTable_;

        // Converts a Tree* structure into a VectorTree structure,
        //  which is efficient for type-checking.
        static void treeToVectorTree(Tree *tree, VectorTree &ret);
        // Build a symbol table from the parse tree.
        static void buildSymbolTable(Tree *tree, SymbolTable &symTable);
        // Typecheck the parse tree to make sure it is semantically correct.
        static void typeCheck(VectorTree const& ret);
};

}

#endif//SEMANTIC_PARSE_H
