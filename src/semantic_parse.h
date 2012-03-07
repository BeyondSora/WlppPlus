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
    typedef std::map<std::string, Type> SymbolTable;
    typedef std::map<std::string, SymbolTable> SymbolTables;
    public:
        explicit ParseTree(Tree* tree);
        // Convert SymbolTables to string format.
        std::string symTablesToString();
    private:
        VectorTree vecTree_;
        SymbolTable fcnTable_;
        SymbolTables symTables_;

        // Converts a Tree* structure into a VectorTree structure,
        //  which is efficient for type-checking.
        static void treeToVectorTree(Tree *tree, VectorTree &ret);
        // Build symbol tables for the whole source code from the parse tree.
        static void buildSymbolTables(VectorTree &vecTree,
                                      SymbolTables &symTables,
                                      SymbolTable &fcnTable);
        // Build symbol table for one function, and
        //  returns the id of the function.
        static std::string buildSymbolTable(VectorTree &vecTree,
                                            SymbolTable &symTable);
        // Add new member to the std::map.
        // Intended for both SymbolTable and SymbolTables type.
        template <typename Map, // must be std::map
                  typename Key,
                  typename Value>
        static void addNew(Map &map, Key const& key, Value const& val);
        // Return Type information about the tree.
        // Intended for Type_Exp only
        static Type getType(VectorTree &vecTree);
        // Return Type information about the tree.
        // Intended for deducing type information related to
        //  symTables_ and fcnTable_.
        Type getType(VectorTree &vecTree, std::string fcnName);
        // Typecheck the parse tree to make sure it is semantically correct.
        void typeCheck(VectorTree &ret);
};

}

#endif//SEMANTIC_PARSE_H
