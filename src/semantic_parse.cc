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
    buildSymbolTables(vecTree_, symTables_, fcnTable_);

    // Type Check
    typeCheck(vecTree_);
}

std::string ParseTree::symTablesToString()
{
    std::string ret;
    for (SymbolTables::iterator i = symTables_.begin();
         i != symTables_.end();
         ++i) {
        ret += "Function: \"" + i->first + "\" -> " +
               typeToString(fcnTable_[i->first]) + "\n";

        for (SymbolTable::iterator j = i->second.begin();
             j != i->second.end();
             ++j) {
            ret += j->first + ": " + typeToString(j->second) + "\n";
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
                                  SymbolTables &symTables,
                                  SymbolTable &fcnTable)
{
    switch (vecTree.rule) {
        default:
            return;

        case Start_Exp_Proc:
            buildSymbolTables(vecTree.leaves[1], symTables, fcnTable);
            break;
        case Procs_Exp_ProcW_Procs: // fall-through
        case Procs_Exp_Procs_Proc:
            buildSymbolTables(vecTree.leaves[0], symTables, fcnTable);
            buildSymbolTables(vecTree.leaves[1], symTables, fcnTable);
            break;

        // Build symbol table for each function.
        case ProcW_Exp: // fall-through
        case Proc_Exp:
            // Build symbol table for the function itself.
            SymbolTable symTable;
            std::string funcName = buildSymbolTable(vecTree, symTable);
            addNew<SymbolTables, std::string, SymbolTable>
                  (symTables, funcName, symTable);

            // Index this function and its return type into fcnTable_;
            Type fcnType = (vecTree.rule == ProcW_Exp ?
                            INT : getType(vecTree.leaves[0]));
            addNew<SymbolTable, std::string, Type>
                  (fcnTable, funcName, fcnType);
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
                symType = kindToType(symKind, common::STAR);
            }
            else {  // size == 1
                symType = kindToType(symKind);
            }
            addNew<SymbolTable, std::string, Type>
                  (symTable, symName, symType);
            break;
    }
    return "";
}

template <typename Map,
          typename Key,
          typename Value>
void ParseTree::addNew(Map &map, Key const& key, Value const& val)
{
    if (!map.count(key)) {
        map[key] = val;
    }
    else {
        throw new error::ErrorObject(error::EXISTS_DUPLICATE_SYMBOL,
                "semantic_parse::ParseTree::addNew"
                "(Map &, Key const&, Value const&)",
                "There already exists an identical symbol: \"" +
                common::toString(key) + "\".");
    }
}

Type ParseTree::getType(VectorTree &vecTree)
{
    Type type;
    switch (vecTree.rule) {
        default:
            throw new error::ErrorObject(error::NO_MATCHING_PROD_RULE,
                    "semantic_parse::ParseTree::getType(VectorTree &)",
                    "No production rule matched!");

        case Type_Exp_Intk: type = INT; break;
        case Type_Exp_IntkStar: type = INT_STAR; break;
        case Type_Exp_Chark: type = CHAR; break;
        case Type_Exp_CharkStar: type = CHAR_STAR; break;
    }
    return type;
}

Type ParseTree::checkExprType(VectorTree &vecTree, std::string const& fcnName)
{
    Type type;
    Type typeExpr, typeTerm;
    switch (vecTree.rule) {
        default:
            throw new error::ErrorObject(error::NO_MATCHING_PROD_RULE,
                    "semantic_parse::ParseTree::checkExprType"
                    "(VectorTree &, std::string const&)",
                    "No production rule matched!");

        case Expr_Exp_Term:
            type = checkTermType(vecTree.leaves[0], fcnName);
            break;
        case Expr_Exp_Plus:
            typeExpr = checkExprType(vecTree.leaves[0], fcnName);
            typeTerm = checkTermType(vecTree.leaves[2], fcnName);
            if (typeExpr == INT && typeTerm == INT) {
                type = INT;
            }
            else if ((typeExpr == INT_STAR && typeTerm == INT) ||
                    (typeExpr == INT && typeTerm == INT_STAR)) {
                type = INT_STAR;
            }
            else if ((typeExpr == CHAR_STAR && typeTerm == INT) ||
                    (typeExpr == INT && typeTerm == CHAR_STAR)) {
                type = CHAR_STAR;
            }
            else {
                throw new error::ErrorObject(error::EXPR_FAULT,
                        "semantic_parse::ParseTree::checkExprType"
                        "(VectorTree &, std::string const&)",
                        "ProductionRule \"Expr_Exp_Plus\" only allows: "
                        "(int + int), (int* + int), (int + int*), "
                        "(char* + int), and (int + char*).");
            }
            break;
        case Expr_Exp_Minus:
            typeExpr = checkExprType(vecTree.leaves[0], fcnName);
            typeTerm = checkTermType(vecTree.leaves[2], fcnName);
            if ((typeExpr == INT && typeTerm == INT) ||
               (typeExpr == INT_STAR && typeTerm == INT_STAR) ||
               (typeExpr == CHAR_STAR && typeTerm == CHAR_STAR)) {
                type = INT;
            }
            else if (typeExpr == INT_STAR && typeTerm == INT) {
                type = INT_STAR;
            }
            else if (typeExpr == CHAR_STAR && typeTerm == CHAR) {
                type = CHAR_STAR;
            }
            else {
                throw new error::ErrorObject(error::EXPR_FAULT,
                        "semantic_parse::ParseTree::checkExprType"
                        "(VectorTree &, std::string const&)",
                        "ProductionRule \"Expr_Exp_Plus\" only allows: "
                        "(int - int), (int* - int), (int* - int*), "
                        "(char* - int), and (char* - char*).");
            }
            break;
    }
    return type;
}

Type ParseTree::checkTermType(VectorTree &vecTree, std::string const& fcnName)
{
    Type type;
    switch (vecTree.rule) {
        default:
            throw new error::ErrorObject(error::NO_MATCHING_PROD_RULE,
                    "semantic_parse::ParseTree::checkTermType"
                    "(VectorTree &, std::string const&)",
                    "No production rule matched!");

        case Term_Exp_Ftor:
            type = checkFtorType(vecTree.leaves[0], fcnName);
            break;
        case Term_Exp_Star:     // fall-through
        case Term_Exp_Slash:    // fall-through
        case Term_Exp_Pct:
            if (checkTermType(vecTree.leaves[0], fcnName) == INT &&
                checkTermType(vecTree.leaves[2], fcnName) == INT) {
                type = INT;
            }
            else {
                throw new error::ErrorObject(error::TERM_FAULT,
                        "semantic_parse::ParseTree::checkTermType"
                        "(VectorTree &vecTree, std::string const&)",
                        "Multiplication/Division/Mod "
                        "only allowed for integers.");
            }
            break;
    }
    return type;
}

Type ParseTree::checkFtorType(VectorTree &vecTree, std::string const& fcnName)
{
    Type type;
    switch (vecTree.rule) {
        default:
            throw new error::ErrorObject(error::NO_MATCHING_PROD_RULE,
                    "semantic_parse::ParseTree::checkFtorType"
                    "(VectorTree &, std::string const&)",
                    "No production rule matched!");

        case Ftor_Exp_Id:
            type = symTables_[fcnName][vecTree.leaves[0].token.lexeme];
            break;
        case Ftor_Exp_Int:
            type = INT;
            break;
        case Ftor_Exp_Char:
            type = CHAR;
            break;
        case Ftor_Exp_Nul:
            type = NUL;
            break;
        case Ftor_Exp_Expr:
            type = checkExprType(vecTree.leaves[1], fcnName);
            break;
        case Ftor_Exp_Addr:
            switch (checkLvalType(vecTree.leaves[1], fcnName)) {
                default:
                    throw new error::ErrorObject(error::FTOR_FAULT,
                            "semantic_parse::ParseTree::checkFtorType"
                            "(VectorTree &, std::string const&)",
                            "Can only take the address of an int or char");

                case INT:
                    type = INT_STAR;
                    break;
                case CHAR:
                    type = CHAR_STAR;
                    break;
            }
            break;
        case Ftor_Exp_Ptr:
            switch (checkLvalType(vecTree.leaves[1], fcnName)) {
                default:
                    throw new error::ErrorObject(error::FTOR_FAULT,
                            "semantic_parse::ParseTree::checkFtorType"
                            "(VectorTree &, std::string const&)",
                            "Can only take the dereference an int* or char*");

                case INT_STAR:
                    type = INT;
                    break;
                case CHAR_STAR:
                    type = CHAR;
                    break;
            }
            break;
    }
    return type;
}

Type ParseTree::checkLvalType(VectorTree &vecTree, std::string const& fcnName)
{
    Type type;
    switch (vecTree.rule) {
        default:
            throw new error::ErrorObject(error::NO_MATCHING_PROD_RULE,
                    "semantic_parse::ParseTree::checkLvalType"
                    "(VectorTree &, std::stringc onst&)",
                    "No production rule matched!");

        case Lval_Exp_Id:
            type = symTables_[fcnName][vecTree.leaves[0].token.lexeme];
            break;
        case Lval_Exp_Ptr:
            switch (checkFtorType(vecTree.leaves[1], fcnName)) {
                default:
                    throw new error::ErrorObject(error::LVAL_FAULT,
                            "semantic_parse::ParseTree::checkLvalType"
                            "(VectorTree &, std::string const&)",
                            "Can only take the dereference an int* or char*");

                case INT_STAR:
                    type = INT;
                    break;
                case CHAR_STAR:
                    type = CHAR;
                    break;
            }
            break;
        case Lval_Exp_Lval:
            type = checkLvalType(vecTree.leaves[1], fcnName);
            break;
    }
    return type;
}

void ParseTree::typeCheck(VectorTree &ret, std::string const& fcnName)
{
    switch (ret.rule) {
        default:    // Do nothing.
            break;

        case Procs_Exp_ProcW_Procs: // fall-through
        case Procs_Exp_Procs_Proc:
            typeCheck(ret.leaves[0]);
            // fall-through
        case Start_Exp_Proc:
            typeCheck(ret.leaves[1]);
            break;
        case ProcW_Exp: // fall-through
        case Proc_Exp:
            typeCheck(ret.leaves[8], ret.leaves[1].token.lexeme);
            typeCheck(ret.leaves[9], ret.leaves[1].token.lexeme);

            if (checkExprType(ret.leaves[11], ret.leaves[1].token.lexeme) !=
               (ret.rule == ProcW_Exp ? INT : getType(ret.leaves[0]))) {
                throw new error::ErrorObject(error::RET_NO_MATCH,
                        "semantic_parse::ParseTree::typeCheck"
                        "(VectorTree &, std::string const&)",
                        "Return type does not match function signature.");
            }
            break;
        case Dcls_Exp_Assign:
            typeCheck(ret.leaves[0], fcnName);
            if (getType(ret.leaves[1].leaves[0]) !=
                checkExprType(ret.leaves[3], fcnName)) {
                throw new error::ErrorObject(error::LHS_RHS_NOT_SAME,
                        "semantic_parse::ParseTree::typeCheck"
                        "(VectorTree &, std::string const&)",
                        "Cannot assign, lhs and rhs types are not the same");
            }
            break;
        case Stmnts_Exp_Stmnts_Stmnt:
            typeCheck(ret.leaves[0], fcnName);
            typeCheck(ret.leaves[1], fcnName);
            break;
        case Stmnt_Exp_Assign:
            if (checkLvalType(ret.leaves[0], fcnName) !=
                checkExprType(ret.leaves[2], fcnName)) {
                throw new error::ErrorObject(error::LHS_RHS_NOT_SAME,
                        "semantic_parse::ParseTree::typeCheck"
                        "(VectorTree &, std::string const&)",
                        "Cannot assign, lhs and rhs types are not the same");
            }
            break;
        case Stmnt_Exp_If:
            typeCheck(ret.leaves[2], fcnName);
            typeCheck(ret.leaves[5], fcnName);
            break;
        case Stmnt_Exp_If_Else:
            typeCheck(ret.leaves[2], fcnName);
            typeCheck(ret.leaves[5], fcnName);
            typeCheck(ret.leaves[9], fcnName);
            break;
        case Stmnt_Exp_While:
            typeCheck(ret.leaves[2], fcnName);
            typeCheck(ret.leaves[5], fcnName);
            break;
        case Tests_Exp_Tests_Test:
            typeCheck(ret.leaves[0], fcnName);
            typeCheck(ret.leaves[2], fcnName);
            break;
        case Tests_Exp_Test:
            typeCheck(ret.leaves[0], fcnName);
            break;
        case Test_Exp_Eq:   // fall-through
        case Test_Exp_Ne:   // fall-through
        case Test_Exp_Lt:   // fall-through
        case Test_Exp_Le:   // fall-through
        case Test_Exp_Ge:   // fall-through
        case Test_Exp_Gt:
            if (!((checkExprType(ret.leaves[0], fcnName) == INT &&
                  checkExprType(ret.leaves[2], fcnName) == INT) ||
                  (checkExprType(ret.leaves[0], fcnName) == CHAR &&
                  checkExprType(ret.leaves[2], fcnName) == CHAR))) {
                throw new error::ErrorObject(error::COMPARISON_FAULT,
                        "semantic_parse::ParseTree::typeCheck"
                        "(VectorTree &, std::string const&)",
                        "Comparison only allowed between INT or between CHAR");
            }
            break;
    }
}

///

}
