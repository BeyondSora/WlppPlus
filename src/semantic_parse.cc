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
            addNewSymbolTable(symTables, symTable, funcName);
            break;
    }
}

std::string ParseTree::buildSymbolTable(VectorTree &vecTree,
                                        SymbolTable &symTable)
{
}

void ParseTree::addNewSymbolTable(SymbolTables &symTables,
                                  SymbolTable &symTable,
                                  std::string funcName)
{
}

void ParseTree::typeCheck(VectorTree &ret)
{
    //switch (ret.rule) {
    //}
}

///

}
