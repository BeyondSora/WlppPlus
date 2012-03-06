/******************************************************************************
 * Copyright (C) 2012 Jimmy Lu
 ******************************************************************************/

#include "semantic_parse.h"

#include "error.h"

namespace semantic_parse {

//
// ParseTree class

ParseTree::ParseTree(Tree *tree) : ParseTreeInterface(tree)
{
    // Build VectorTree.
    vecTree_.rule = tree_->rule;
    vecTree_.token = tree_->token;
    treeToVectorTree(tree_->next, vecTree_);

    // Build symbol table.
    buildSymbolTables(vecTree_, symTables_);
}

std::string ParseTree::symTablesToString()
{
    std::string ret;
    for (SymbolTables::iterator i = symTables_.begin();
         i != symTables_.end();
         ++i) {
        ret += "Function: \"" + i->first + "\"\n";

        for (SymbolTable::iterator j = i->second.begin();
             j != i->second.end();
             ++j) {
            ret += j->first + ": " + getType(j->second) + "\n";
        }
    }
    return ret;
}

void ParseTree::treeToVectorTree(Tree *tree, VectorTree &ret)
{
    for (; tree != NULL; tree = tree->next) {
        VectorTree leaf;
        leaf.rule = tree->rule;
        leaf.token = tree->token;
        if (tree->down != NULL) {
            treeToVectorTree(tree->down, leaf);
        }

        ret.leaves.push_back(leaf);
    }
}

void ParseTree::buildSymbolTables(VectorTree &vecTree,
                                  SymbolTables &symTables)
{
    switch (vecTree.rule) {
        default:
            return;

        case Start_Exp_Proc:
            buildSymbolTables(vecTree.leaves[1], symTables);
            break;
        case Procs_Exp_ProcW_Procs: // fall-through
        case Procs_Exp_Procs_Proc:
            buildSymbolTables(vecTree.leaves[0], symTables);
            buildSymbolTables(vecTree.leaves[1], symTables);
            break;

        // Build symbol table for each function.
        case ProcW_Exp: // fall-through
        case Proc_Exp:
            SymbolTable symTable;
            std::string funcName = buildSymbolTable(vecTree, symTable);
            addNew<SymbolTables, std::string, SymbolTable>
                  (symTables, funcName, symTable);
            break;
    }
}

std::string ParseTree::buildSymbolTable(VectorTree &vecTree,
                                        SymbolTable &symTable)
{
    switch (vecTree.rule) {
        default:
            return "";

        case ProcW_Exp:
            buildSymbolTable(vecTree.leaves[3], symTable);
            buildSymbolTable(vecTree.leaves[5], symTable);
            buildSymbolTable(vecTree.leaves[8], symTable);
            return "wain";  // Main function's ID is always wain
        case Proc_Exp:
            buildSymbolTable(vecTree.leaves[3], symTable);
            buildSymbolTable(vecTree.leaves[5], symTable);
            buildSymbolTable(vecTree.leaves[8], symTable);
            return vecTree.leaves[1].token.lexeme; // Return function name.

        case Dcls_Exp_Assign:   // fall-through
        case Dcls_Exp_NoAssign:
            buildSymbolTable(vecTree.leaves[0], symTable);
            buildSymbolTable(vecTree.leaves[1], symTable);
            break;
        case Dcl_Exp:
            std::string symName = vecTree.leaves[1].token.lexeme;
            common::Kind symKind = vecTree.leaves[0].leaves[0].token.kind;
            Type symType;
            if (vecTree.leaves[0].leaves.size() == 2) {
                if (symKind == common::INTK) {
                    symType = INT_STAR;
                }
                else {  // == common::CHARK
                    symType = CHAR_STAR;
                }
            }
            else {  // size == 1
                if (symKind == common::INTK) {
                    symType = INT;
                }
                else {  // == common::CHARK
                    symType = CHAR;
                }
            }
            addNew<SymbolTable, std::string, Type>
                  (symTable, symName, symType);
            break;
    }
    return "";
}

template <typename Map, typename Key, typename Value>
void ParseTree::addNew(Map &map, Key const& key, Value const& val)
{
    if (!map.count(key)) {
        map[key] = val;
    }
    else {
        throw "error! Duplicates!\n";
    }
}

void ParseTree::typeCheck(VectorTree &ret)
{
    //switch (ret.rule) {
    //}
}

///

}
