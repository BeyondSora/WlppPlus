/******************************************************************************
 * Copyright (C) 2012 Jimmy Lu
 *
 * This namespace contains what is shared across
 *  context-free and context-sensitive(semantic) parsing.
 *
 * More implementation details are placed in .cc file,
 *  but end users should have no need for those.
 ******************************************************************************/

#ifndef PARSE_COMMON_H
#define PARSE_COMMON_H

#include <string>
#include <vector>

#include "common.h"

// This namespace is forward declaration.
// ParseTree is friend class of Tree.
namespace context_free_parse { class ParseTree; }
namespace semantic_parse { class ParseTree; }

namespace parse_common {

// The heart of parsing :-)
//  this section defines the grammar of WlppPlus.
//
// Terminal symbols: the set of valid tokens as defined in common::Kind
//                   and BOF & EOF.
// Non-Terminal symbols: Start, proceduresAll, procedureWain, procedures,
//                       procedure, type, dcls, dcl, statements, statement,
//                       tests, test, expr, term, factor, lvalue.
// Start symbol: Start
// Production Rules: as defined below
enum ProductionRule {
    Start_Exp_Proc,         // Start -> bof proceduresAll eof

    Procs_Exp_ProcW_Procs,  // proceduresAll -> procedureWain procedures
    Procs_Exp_Procs_Proc,   // procedures -> procedures procedure
    Procs_Exp_Nothing,      // procedures -> Nothing

    ProcW_Exp,      // procedureWain -> INTK WAIN LPAREN dcl COMMA dcl RPAREN
                    //                  LBRACE
                    //                  dcls statements RETURN expr SEMI
                    //                  RBRACE
    Proc_Exp,       // procedure -> type ID LPAREN dcl COMMA dcl RPAREN
                    //              LBRACE
                    //              dcls statements RETURN expr SEMI
                    //              RBRACE

    Type_Exp_Intk,          // type -> INTK
    Type_Exp_IntkStar,      // type -> INTK STAR
    Type_Exp_Chark,         // type -> CHARK
    Type_Exp_CharkStar,     // type -> CHARK STAR

    Dcls_Exp_Assign,        // dcls -> dcls dcl BECOMES expr SEMI
    Dcls_Exp_NoAssign,      // dcls -> dcls dcl SEMI
    Dcls_Exp_Nothing,       // dcls -> Nothing
    Dcl_Exp,                // dcl -> type ID

    Stmnts_Exp_Stmnts_Stmnt,// statements -> statements statement
    Stmnts_Exp_Nothing,     // statements -> Nothing
    Stmnt_Exp_Assign,       // statement -> lvalue BECOMES expr SEMI
    Stmnt_Exp_If,           // statement -> IF LPAREN tests AND test RPAREN
                            //               LBRACE statements RBRACE
    Stmnt_Exp_If_Else,      // statement -> IF LPAREN tests AND test RPAREN
                            //               LBRACE statements RBRACE
                            //               ELSE LBRACE statements RBRACE
    Stmnt_Exp_While,        // statement -> WHILE LPAREN tests AND test RPAREN
                            //               LBRACE statements RBRACE

    Tests_Exp_Tests_Test,   // tests -> tests AND test
    Tests_Exp_Test,         // tests -> test

    Test_Exp_Eq,            // test -> expr EQ expr
    Test_Exp_Ne,            // test -> expr NE expr
    Test_Exp_Lt,            // test -> expr LT expr
    Test_Exp_Le,            // test -> expr LE expr
    Test_Exp_Ge,            // test -> expr GE expr
    Test_Exp_Gt,            // test -> expr GT expr

    Expr_Exp_Term,          // expr -> term
    Expr_Exp_Plus,          // expr -> expr PLUS term
    Expr_Exp_Minus,         // expr -> expr MINUS term

    Term_Exp_Ftor,          // term -> factor
    Term_Exp_Star,          // term -> term STAR factor
    Term_Exp_Slash,         // term -> term SLASH factor
    Term_Exp_Pct,           // term -> term PCT factor

    Ftor_Exp_Id,            // factor -> ID
    Ftor_Exp_Int,           // factor -> INT
    Ftor_Exp_Char,          // factor -> CHAR
    Ftor_Exp_Nul,           // factor -> NUL
    Ftor_Exp_Expr,          // factor -> LPAREN expr RPAREN
    Ftor_Exp_Addr,          // factor -> AMP lvalue
    Ftor_Exp_Ptr,           // factor -> STAR factor

    Lval_Exp_Id,            // lvalue -> ID
    Lval_Exp_Ptr,           // lvalue -> STAR factor
    Lval_Exp_Lval,          // lvalue -> LPAREN lvalue RPAREN

    NUL_RULE,               // NOT A PRODUCTION RULE
};

// Variable Types
enum Type {
    INT,
    INT_STAR,
    CHAR,
    CHAR_STAR,
};

std::string getType(Type type);

// Base unit for ParseTree.
// There should not be a need to create an instance of this class.
class Tree {
    friend class ParseTreeInterface;
    friend class context_free_parse::ParseTree;
    friend class semantic_parse::ParseTree;
    public:
        Tree();
        ~Tree();
    private:
        ProductionRule rule;
        common::Token token;
        Tree *prev;
        Tree *next;
        Tree *down;
        void connect(Tree *rhs);    // this->next <---> rhs->prev
        void disconnect(Tree *rhs); // this->next <-/-> rhs->prev
};

// The foundation for context_free_parse::ParseTree
//  and semantic_parse::ParseTree down the road.
// Never instantiate an instance of this class by itself!
class ParseTreeInterface {
    public:
        virtual ~ParseTreeInterface();
        Tree* operator*();  // Returns tree_ of Type Tree*.
        Tree* move();       // Returns tree_ and then nullify it.
        std::string toString();
    protected:
        Tree *tree_;

        ParseTreeInterface();
        ParseTreeInterface(Tree *tree);
        // Convert a tree structure into string in CFG format.
        static void convTreeToString(Tree *root, std::string &str);
};

}

#endif//PARSE_COMMON_H
