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
    vecTree_.rule = tree_->rule;
    vecTree_.token = tree_->token;
    treeToVectorTree(tree_->next, vecTree_);
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

void ParseTree::buildSymbolTable(Tree *tree, SymbolTable &symTable)
{
}

void ParseTree::typeCheck(VectorTree const& ret)
{
    //switch (ret.rule) {
    //}
}

///

}
