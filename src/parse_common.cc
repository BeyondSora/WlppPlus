#include "parse_common.h"

namespace parse_common {

std::string translateProductionRule(const ProductionRule &rule);

std::string translateProductionRule(const ProductionRule &rule)
{
    std::string translation;
    switch (rule) {
        default: throw "Rule not found!\n"; break;

        case NUL_RULE: translation = ""; break;     // NOT A PRODUCTION RULE

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
                          "INTK WAIN LPAREN dcl COMMA dcl RPAREN "
                          "LBRACE "
                          "dcls statements RETURN expr SEMI "
                          "RBRACE";
            break;
        case Proc_Exp:
            translation = "procedure "
                          "type ID LPAREN dcl COMMA dcl RPAREN "
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
            translation = "dcls dcls dcl SEMI";
            break;
        case Dcls_Exp_Nothing:
            translation = "dcls";
            break;
        case Dcl_Exp:
            translation = "dcl type ID";
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
                          "IF LPAREN tests RPAREN "
                          "LBRACE statements RBRACE";
            break;
        case Stmnt_Exp_If_Else:
            translation = "statement "
                          "IF LPAREN tests RPAREN "
                          "LBRACE statements RBRACE "
                          "ELSE LBRACE statements RBRACE";
            break;
        case Stmnt_Exp_While:
            translation = "statement "
                          "WHILE LPAREN tests RPAREN "
                          "LBRACE statements RBRACE";
            break;
        case Tests_Exp_Tests_Test:
            translation = "tests tests AND test";
            break;
        case Tests_Exp_Test:
            translation = "tests test";
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

//
// Tree class

Tree::Tree() : rule(NUL_RULE), prev(NULL), next(NULL), down(NULL) {}

Tree::~Tree()
{
    delete next;
    delete down;
}

void Tree::connect(Tree *rhs)
{
    this->next = rhs;
    rhs->prev = this;
}

void Tree::disconnect(Tree *rhs)
{
    this->next = NULL;
    rhs->prev = NULL;
}

///

//
// ParseTreeInterface class

ParseTreeInterface::ParseTreeInterface() {}

ParseTreeInterface::ParseTreeInterface(Tree *tree) : tree_(tree) {}

ParseTreeInterface::~ParseTreeInterface()
{
    delete tree_;
}

Tree* ParseTreeInterface::operator*()
{
    return tree_;
}

Tree* ParseTreeInterface::move()
{
    Tree *ret = tree_;
    tree_ = NULL;
    return ret;
}

std::string ParseTreeInterface::toString()
{
    if (tree_ == NULL) {
        throw "ParseTreeInterface::toString() - tree_ is NULL!\n";
    }
    std::string str;
    convTreeToString(tree_, str);
    return str;
}

void ParseTreeInterface::convTreeToString(Tree *root, std::string &str)
{
    if (root != NULL) {
        std::string ruleStr = translateProductionRule(root->rule);
        str += (ruleStr == "" ? "" : ruleStr + "\n");
    }
    if (root->down != NULL) {
        convTreeToString(root->down, str);
    }
    if (root->next != NULL) {
        convTreeToString(root->next, str);
    }
}

///

}
