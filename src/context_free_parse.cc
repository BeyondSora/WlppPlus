#include "context_free_parse.h"

#include <iostream>

#include "lr1_rules.h"

namespace context_free_parse {

using namespace lr1_rules;  // LR1_RULES is only in another file
                            //  because of its large number of code.

unsigned getReductionSize(ProductionRule const& rule)
{
    unsigned size = 0;
    switch (rule) {
        default:
            throw "parse_common::getReductionSize"
                  " - Rule not found!\n";
            break;

        case Procs_Exp_Nothing:     // fall-through
        case Dcls_Exp_Nothing:      // fall-through
        case Stmnts_Exp_Nothing:    // fall-through
            size = 0;
            break;
        case Type_Exp_Intk:     // fall-through
        case Type_Exp_Chark:    // fall-through
        case Tests_Exp_Test:    // fall-through
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
            size = 7;
            break;
        case Stmnt_Exp_If_Else:
            size = 11;
            break;
        case ProcW_Exp: // fall-through
        case Proc_Exp:
            size = 14;
            break;
    }
    return size;
}

common::Kind getReductionKind(ProductionRule const& rule)
{
    common::Kind kind;
    switch (rule) {
        default:
            throw "parse_common::getReductionKind"
                  " - Rule not found!\n";
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
        case Tests_Exp_Test:        // fall-through
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

inline void Tree::connect(Tree *rhs)
{
    this->next = rhs;
    rhs->prev = this;
}

inline void Tree::disconnect(Tree *rhs)
{
    this->next = NULL;
    rhs->prev = NULL;
}

// Turn Tokens into one single line of tokenized input,
//  and also prepends BOF and EOF to the source code.
// All Common::COMMENT tokens are removed during this process.
// Need some rework in the future to improve efficiency.
void tokensLinearize(std::vector<common::Token> &src,
                     common::Tokens const& tokens);

// Parsing method similar to the one I used in CS241 assignment 8.
// Need to be revised in the future!!
Tree* build_parse_tree(common::Tokens const& tokens)
{
    std::vector<common::Token> src;
    tokensLinearize(src, tokens);

    Tree *root = new Tree();

    std::vector<unsigned> statesStack;
    common::Kind curSym;
    unsigned curState = 0;

    root->token.kind = common::Start;

    Tree *curNode = root;

    bool test = false;
    for (unsigned i = 0; i < src.size(); ++i) {
        common::Token curToken = src[i];
        if (!test) curSym = curToken.kind;
        LR1ParseRule lr1Rule = getLR1Rule(curState, curSym);

        if (test == true) {
            i--;
            test = false;
        }

        Tree *newNode;
        switch (lr1Rule.type) {
            case SHIFT:
                if (curSym == curToken.kind) {
                    newNode = new Tree();
                    newNode->token = curToken;
                    curNode->connect(newNode);
                    curNode = curNode->next;
                }

                statesStack.push_back(curState);

                curState = lr1Rule.next;


                break;
            case REDUCE:
                /*
                std::cout << "Current State: " << curState << std::endl;
                std::cout << "Reduction: " << lr1Rule.next << std::endl;
                std::cout << "States Stack: " << statesStack.size() << std::endl;
                std::cout << "Current Token: " << curToken.getKind() << std::endl;
                std::cout << "i: " << i << std::endl;
                std::cout << translateProductionRule(
                             (ProductionRule)lr1Rule.next);
                std::cout << std::endl;
                */

                curSym = getReductionKind((ProductionRule)lr1Rule.next);

                unsigned size = getReductionSize((ProductionRule)lr1Rule.next);
                for (; size > 0; --size) {
                    curNode = curNode->prev;

                    curState = statesStack[statesStack.size() - 1];
                    statesStack.pop_back();
                }

                common::Token reducToken;
                reducToken.kind = curSym;

                newNode = new Tree();
                newNode->token = reducToken;

                newNode->down = curNode->next;

                if (curNode->next != NULL) curNode->disconnect(curNode->next);
                curNode->connect(newNode);
                curNode = curNode->next;

                i--;
                test = true;

                break;
        }
    }
    return root;
}

void tokensLinearize(std::vector<common::Token> &src,
                     common::Tokens const& tokens)
{
    common::Token bof = { common::bof, "bof" };
    src.push_back(bof);

    for (unsigned i = 0; i < tokens.size(); ++i)
        for (unsigned j = 0; j < tokens[i].size(); ++j)
            if (tokens[i][j].kind != common::COMMENT)
                src.push_back(tokens[i][j]);

    common::Token eof = { common::eof, "eof" };
    src.push_back(eof);
}

void print_parse_tree(Tree *root)
{
    if (root != NULL) {
        if (root->prev == NULL) {
            for (Tree *it = root; it != NULL; it = it->next) {
                std::cout << it->token.getKind() << " ";
            }
            std::cout << std::endl;
        }
        if (root->down != NULL)
            if (root->down->prev == NULL) {
                std::cout << root->token.getKind() << " ";
            }
            print_parse_tree(root->down);
        if (root->next != NULL)
            if (root->next->prev == NULL) {
                std::cout << root->token.getKind() << " ";
            }
            print_parse_tree(root->next);
    }
}

}
