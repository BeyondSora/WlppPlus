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
    typedef std::map<std::string, std::string> SymbolTable; // function-wide
    typedef std::map<std::string, SymbolTable> SymbolTables;// sourcecode-wide
    public:
        explicit ParseTree(Tree* tree);
    private:
        VectorTree vecTree_;
        SymbolTables symTables_;

        // Converts a Tree* structure into a VectorTree structure,
        //  which is efficient for type-checking.
        static void treeToVectorTree(Tree *tree, VectorTree &ret);
        // Build symbol tables for the whole source code from the parse tree.
        static void buildSymbolTables(VectorTree &vecTree,
                                      SymbolTables &symTables);
        // Build symbol table for one function, and
        //  returns the id of the function.
        static std::string buildSymbolTable(VectorTree &vecTree,
                                     SymbolTable &symTable);
        // Add new symbol table to the SymbolTables.
        static void addNewSymbolTable(SymbolTables &symTables,
                                      SymbolTable &symTable,
                                      std::string funcName);
        // Typecheck the parse tree to make sure it is semantically correct.
        static void typeCheck(VectorTree &ret);
};

}

#endif//SEMANTIC_PARSE_H
