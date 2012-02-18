/******************************************************************************
 * This namespace contains what is shared across
 *  context-free and context-sensitive parsing.
 *
 * More implementation details are placed in .cc file,
 *  but end users should have no need for those.
 ******************************************************************************/
#ifndef PARSE_COMMON_H
#define PARSE_COMMON_H

#include <string>
#include <vector>

namespace parse_common {

// The heart of parsing :-)
//  this section defines the grammar of WlppPlus.
//
// Terminal symbols: the set of valid tokens as defined in common::Kind
//                   and BOF & EOF.
// Non-Terminal symbols: Start, proceduresAll, procedureWain, procedures, procedure,
//                       type, dcls, dcl, statements, statement, tests, test,
//                       expr, term, factor, lvalue.
// Start symbol: Start
// Production Rules: as defined below
enum ProductionRule {
    Start_Exp_Proc,         // Start -> BOF procedures EOF

    Procs_Exp_ProcW_Procs,  // proceduresAll -> procedureWain procedures
    Procs_Exp_Procs_Proc,   // procedures -> procedures procedure
    Procs_Exp_Nothing,      // procedures -> Nothing

    ProcW_Exp,              // procedureWain -> INTK WAIN LPAREN dcls RPAREN
                            //                  LBRACE
                            //                  dcls statements RETURN expr SEMI
                            //                  RBRACE
    Proc_Exp,               // procedure -> type ID LPAREN dcls RPAREN
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
    Stmnts_Exp_Nothing,     // statements -> NULL
    Stmnt_Exp_Assign,       // statement -> lvalue BECOMES expr SEMI
    Stmnt_Exp_If,           // statement -> IF LPAREN tests AND test RPAREN
                            //               LBRACE statements RBRACE
    Stmnt_Exp_If_ELSE,      // statement -> IF LPAREN tests AND test RPAREN
                            //               LBRACE statements RBRACE
                            //               ELSE LBRACE statements RBRACE
    Stmnt_Exp_While,        // statement -> WHILE LPAREN tests AND test RPAREN
                            //               LBRACE statements RBRACE

    Tests_Exp_Tests_Test,   // tests -> tests AND test
    Tests_Exp_Nothing,      // tests -> Nothing

    Test_Exp_Eq,            // test -> expr EQ expr
    Test_Exp_NE,            // test -> expr NE expr
    Test_Exp_LT,            // test -> expr LT expr
    Test_Exp_LE,            // test -> expr LE expr
    Test_Exp_GE,            // test -> expr GE expr
    Test_Exp_GT,            // test -> expr GT expr

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
    Ftor_Exp_Null,          // factor -> NUL
    Ftor_Exp_Expr,          // factor -> LPAREN expr RPAREN
    Ftor_Exp_Addr,          // factor -> AMP lvalue
    Ftor_Exp_Ptr,           // factor -> STAR factor

    Lval_Exp_Id,            // lvalue -> ID
    Lval_Exp_Ptr,           // lvalue -> STAR factor
    Lval_Exp_Lval,          // lvalue -> LPAREN lvalue RPAREN
};

class Tree {
    public:
        Tree();
        ~Tree();
        Tree* getLeaf(int i);
        std::string getID();    // Only terminal symbols have identifiers

    private:
        ProductionRule rule;
        std::vector<std::string> units;
        std::vector<Tree*> leaves;
};

}

#endif//PARSE_COMMON_H
