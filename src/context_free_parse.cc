#include "context_free_parse.h"

#include <cassert>
#include <iostream>

#include "lr1_rules.h"

namespace context_free_parse {

using namespace lr1_rules;  // LR1_RULES is only in another file
                            //  because of its large number of code.

unsigned getReductionSize(const ProductionRule &rule)
{
    unsigned size = 0;
    switch (rule) {
        default:
            assert("parse_common::getReductionSize"
                   " - Rule not found!\n");
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
            assert("parse_common::getReductionKind"
                   " - Rule not found!\n");
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

inline Tree::Tree() : prev(NULL), next(NULL), down(NULL) {}

inline Tree::~Tree()
{
    delete prev;
    delete next;
    delete down;
}

void Tree::connect(Tree *rhs)
{
    assert(rhs != NULL);
    this->next = rhs;
    rhs->prev = this;
}

// Turn Tokens into one single line of tokenized input,
//  and also prepends BOF and EOF to the source code.
// Need some rework in the future to improve efficiency.
void tokensLinearize(std::vector<common::Token> &src,
                     const common::Tokens &tokens);

// Parsing method similar to the one I used in CS241 assignment 8.
// Need to be revised in the future!!
Tree* build_parse_tree(const common::Tokens &tokens)
{
    std::vector<common::Token> src;
    tokensLinearize(src, tokens);

    Tree *root = NULL;

    std::vector<unsigned> statesStack;
    common::Kind curSym;
    unsigned curState = 0;

    Tree *curNode = new Tree();
    curNode->token.kind = common::Start;

    for (unsigned i = 0; i < src.size(); ++i) {
        common::Token curToken = src[i];
        curSym = curToken.kind;
        LR1ParseRule lr1Rule = getLR1Rule(curState, curSym);

        Tree *newNode;
        switch (lr1Rule.type) {
            case SHIFT:
                newNode = new Tree();
                newNode->token = curToken;
                curNode->connect(newNode);
                curNode = curNode->next;

                curState = lr1Rule.next;

                break;
            case REDUCE:
                std::cout << lr1Rule.next << std::endl;
                std::cout << translateProductionRule(
                             (ProductionRule)(lr1Rule.next - 1));
                std::cout << std::endl;


                break;
        }
    }



    return root;
}

void tokensLinearize(std::vector<common::Token> &src,
                     const common::Tokens &tokens)
{
    common::Token bof = { common::bof, "bof" };
    src.push_back(bof);

    for (unsigned i = 0; i < tokens.size(); ++i)
        for (unsigned j = 0; j < tokens[i].size(); ++j)
            src.push_back(tokens[i][j]);

    common::Token eof = { common::eof, "eof" };
    src.push_back(bof);
}

}
