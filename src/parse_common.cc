#include "parse_common.h"

namespace parse_common {

std::string translateProductionRule(const ProductionRule &rule)
{
    std::string translation;
    switch (rule) {
        case Start_Exp_Proc:
            translation = "Start bof proceduresAll eof";
            break;
        case Procs_Exp_ProcW_Procs:
            translation = "proceduresAll procedureWain procedures";
            break;
        case Procs_Exp_Procs_Proc:
            translation = "procedures procedures procedure";
            break;
        case Procs_Exp_Nothing:
            translation = "procedures";
            break;
        case ProcW_Exp:
            translation = "procedureWain "
                          "INTK WAIN LPAREN dcls RPAREN "
                          "LBRACE "
                          "dcls statements RETURN expr SEMI "
                          "RBRACE";
            break;
        case Proc_Exp:
            translation = "procedure "
                          "type ID LPAREN dcls RPAREN "
                          "LBRACE "
                          "dcls statements RETURN expr SEMI "
                          "RBRACE";
            break;
        case Type_Exp_Intk:
            translation = "type INTK";
            break;
        case Type_Exp_IntkStar:
            translation = "type INTK STAR";
            break;
        case Type_Exp_Chark:
            translation = "type CHARK";
            break;
        case Type_Exp_CharkStar:
            translation = "type CHARK STAR";
            break;
        case Dcls_Exp_Assign:
            translation = "dcls dcls dcl BECOMES expr SEMI";
            break;
        case Dcls_Exp_NoAssign:
            translation = "dcls dcl SEMI";
            break;
        case Dcls_Exp_Nothing:
            translation = "dcls";
            break;
        case Dcl_Exp:
            translation = "type ID";
            break;
        case Stmnts_Exp_Stmnts_Stmnt:
            translation = "statements statements statement";
            break;
        case Stmnts_Exp_Nothing:
            translation = "statements";
            break;
        case Stmnt_Exp_Assign:
            translation = "statement lvalue BECOMES expr SEMI";
            break;
        case Stmnt_Exp_If:
            translation = "statement "
                          "IF LPAREN tests AND test RPAREN "
                          "LBRACE statements RBRACE";
            break;
        case Stmnt_Exp_If_Else:
            translation = "statement "
                          "IF LPAREN tests AND test RPAREN "
                          "LBRACE statements RBRACE "
                          "ELSE LBRACE statements RBRACE";
            break;
        case Stmnt_Exp_While:
            translation = "statement "
                          "WHILE LPAREN tests AND test RPAREN "
                          "LBRACE statements RBRACE";
            break;
        case Tests_Exp_Tests_Test:
            translation = "tests tests AND test";
            break;
        case Tests_Exp_Nothing:
            translation = "tests";
            break;
        case Test_Exp_Eq:
            translation = "test expr EQ expr";
            break;
        case Test_Exp_Ne:
            translation = "test expr NE expr";
            break;
        case Test_Exp_Lt:
            translation = "test expr LT expr";
            break;
        case Test_Exp_Le:
            translation = "test expr LE expr";
            break;
        case Test_Exp_Ge:
            translation = "test expr GE expr";
            break;
        case Test_Exp_Gt:
            translation = "test expr GT expr";
            break;
        case Expr_Exp_Term:
            translation = "expr term";
            break;
        case Expr_Exp_Plus:
            translation = "expr expr PLUS term";
            break;
        case Expr_Exp_Minus:
            translation = "expr expr MINUS term";
            break;
        case Term_Exp_Ftor:
            translation = "term factor";
            break;
        case Term_Exp_Star:
            translation = "term term STAR factor";
            break;
        case Term_Exp_Slash:
            translation = "term term SLASH factor";
            break;
        case Term_Exp_Pct:
            translation = "term term PCT factor";
            break;
        case Ftor_Exp_Id:
            translation = "factor ID";
            break;
        case Ftor_Exp_Int:
            translation = "factor INT";
            break;
        case Ftor_Exp_Char:
            translation = "factor CHAR";
            break;
        case Ftor_Exp_Nul:
            translation = "factor NUL";
            break;
        case Ftor_Exp_Expr:
            translation = "factor LPAREN expr RPAREN";
            break;
        case Ftor_Exp_Addr:
            translation = "factor AMP lvalue";
            break;
        case Ftor_Exp_Ptr:
            translation = "factor STAR factor";
            break;
        case Lval_Exp_Id:
            translation = "lvalue ID";
            break;
        case Lval_Exp_Ptr:
            translation = "lvalue STAR factor";
            break;
        case Lval_Exp_Lval:
            translation = "lvalue LPAREN lvalue RPAREN";
            break;
    }
    return translation;
}

unsigned getReductionSize(const ProductionRule &rule)
{
    unsigned size = 0;
    switch (rule) {
        default:
            throw "parse_common::getReductionSize"
                  " - Rule not found!/n";
            break;

        case Procs_Exp_Nothing:     // fall-through
        case Dcls_Exp_Nothing:      // fall-through
        case Stmnts_Exp_Nothing:    // fall-through
        case Tests_Exp_Nothing:
            size = 0;
            break;
        case Type_Exp_Intk:     // fall-through
        case Type_Exp_Chark:    // fall-through
        case Expr_Exp_Term:     // fall-through
        case Term_Exp_Ftor:     // fall-through
        case Ftor_Exp_Id:       // fall-through
        case Ftor_Exp_Int:      // fall-through
        case Ftor_Exp_Char:     // fall-through
        case Ftor_Exp_Nul:      // fall-through
        case Lval_Exp_Id:
            size = 1;
            break;
        case Procs_Exp_ProcW_Procs: // fall-through
        case Procs_Exp_Procs_Proc:  // fall-through
        case Type_Exp_IntkStar:     // fall-through
        case Type_Exp_CharkStar:    // fall-through
        case Dcl_Exp:               // fall-through
        case Stmnts_Exp_Stmnts_Stmnt:   // fall-through
        case Ftor_Exp_Addr: // fall-through
        case Ftor_Exp_Ptr:  // fall-through
        case Lval_Exp_Ptr:
            size = 2;
            break;
        case Start_Exp_Proc:    // fall-through
        case Dcls_Exp_NoAssign: // fall-through
        case Tests_Exp_Tests_Test:  // fall-through
        case Test_Exp_Eq:       // fall-through
        case Test_Exp_Ne:       // fall-through
        case Test_Exp_Lt:       // fall-through
        case Test_Exp_Le:       // fall-through
        case Test_Exp_Ge:       // fall-through
        case Test_Exp_Gt:       // fall-through
        case Expr_Exp_Plus:     // fall-through
        case Expr_Exp_Minus:    // fall-through
        case Term_Exp_Star:     // fall-through
        case Term_Exp_Slash:    // fall-through
        case Term_Exp_Pct:      // fall-through
        case Ftor_Exp_Expr:     // fall-through
        case Lval_Exp_Lval:
            size = 3;
            break;
        case Stmnt_Exp_Assign:
            size = 4;
            break;
        case Dcls_Exp_Assign:
            size = 5;
            break;
        case Stmnt_Exp_If:  // fall-through
        case Stmnt_Exp_While:
            size = 9;
            break;
        case ProcW_Exp: // fall-through
        case Proc_Exp:
            size = 12;
            break;
        case Stmnt_Exp_If_Else:
            size = 13;
            break;
    }
    return size;
}

common::Kind getReductionKind(const ProductionRule &rule)
{
    common::Kind kind;
    switch (rule) {
        default:
            throw "parse_common::getReductionKind"
                  " - Rule not found!/n";
            break;

        case Start_Exp_Proc:
            kind = common::Start;
            break;
        case Procs_Exp_ProcW_Procs:
            kind = common::proceduresAll;
            break;
        case Procs_Exp_Procs_Proc:  // fall-through
        case Procs_Exp_Nothing:
            kind = common::procedures;
            break;
        case ProcW_Exp:
            kind = common::procedureWain;
            break;
        case Proc_Exp:
            kind = common::procedure;
            break;
        case Type_Exp_Intk:     // fall-through
        case Type_Exp_IntkStar: // fall-through
        case Type_Exp_Chark:    // fall-through
        case Type_Exp_CharkStar:
            kind = common::type;
            break;
        case Dcls_Exp_Assign:   // fall-through
        case Dcls_Exp_NoAssign: // fall-through
        case Dcls_Exp_Nothing:
            kind = common::dcls;
            break;
        case Dcl_Exp:
            kind = common::dcl;
            break;
        case Stmnts_Exp_Stmnts_Stmnt:   // fall-through
        case Stmnts_Exp_Nothing:
            kind = common::statements;
            break;
        case Stmnt_Exp_Assign:  // fall-through
        case Stmnt_Exp_If:      // fall-through
        case Stmnt_Exp_If_Else: // fall-through
        case Stmnt_Exp_While:
            kind = common::statement;
            break;
        case Tests_Exp_Tests_Test:  // fall-through
        case Tests_Exp_Nothing:
            kind = common::tests;
            break;
        case Test_Exp_Eq:   // fall-through
        case Test_Exp_Ne:   // fall-through
        case Test_Exp_Lt:   // fall-through
        case Test_Exp_Le:   // fall-through
        case Test_Exp_Ge:   // fall-through
        case Test_Exp_Gt:
            kind = common::test;
            break;
        case Expr_Exp_Term: // fall-through
        case Expr_Exp_Plus: // fall-through
        case Expr_Exp_Minus:
            kind = common::expr;
            break;
        case Term_Exp_Ftor: // fall-through
        case Term_Exp_Star: // fall-through
        case Term_Exp_Slash:// fall-through
        case Term_Exp_Pct:
            kind = common::term;
            break;
        case Ftor_Exp_Id:   // fall-through
        case Ftor_Exp_Int:  // fall-through
        case Ftor_Exp_Char: // fall-through
        case Ftor_Exp_Nul:  // fall-through
        case Ftor_Exp_Expr: // fall-through
        case Ftor_Exp_Addr: // fall-through
        case Ftor_Exp_Ptr:
            kind = common::factor;
            break;
        case Lval_Exp_Id:   // fall-through
        case Lval_Exp_Ptr:  // fall-through
        case Lval_Exp_Lval:
            kind = common::lvalue;
            break;
    }
    return kind;
}

Tree::Tree() {}

Tree::~Tree()
{
    for (unsigned i = 0; i < this->subTrees.size(); ++i)
        delete this->subTrees[i];
}

inline Tree* Tree::getSubTree(const unsigned &i)
{
    return this->subTrees.at(i);
}

inline std::string Tree::getID()
{
    std::string id;
    switch (this->rule) {
        default: throw "Not a terminal symbol\n"; break;

        case Ftor_Exp_Id:       // fall-through
        case Ftor_Exp_Int:      // fall-through
        case Ftor_Exp_Char:     // fall-through
        case Lval_Exp_Id:
                 id = this->units[1];
                 break;
    }
    return id;
}

}
