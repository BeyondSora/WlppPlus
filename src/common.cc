/******************************************************************************
 * Copyright (C) 2012 Jimmy Lu
 ******************************************************************************/

#include "common.h"

#include <cassert>
#include <cstdlib>
#include <sstream>

namespace common {

enum Errcode {
    NOT_AN_INT,
};

//
// Token struct's member functions:
std::string Token::getKind()
{
    std::string kindString;

    switch (this->kind) {
        default:            assert(false);              break;

        // Terminal
        case ID:            kindString = "ID";          break;
        case WAIN:          kindString = "WAIN";        break;
        case IF:            kindString = "IF";          break;
        case ELSE:          kindString = "ELSE";        break;
        case WHILE:         kindString = "WHILE";       break;
        case RETURN:        kindString = "RETURN";      break;
        case INTK:          kindString = "INTK";        break;
        case INT:           kindString = "INT";         break;
        case CHARK:         kindString = "CHARK";       break;
        case CHAR:          kindString = "CHAR";        break;
        case LPAREN:        kindString = "LPAREN";      break;
        case RPAREN:        kindString = "RPAREN";      break;
        case LBRACE:        kindString = "LBRACE";      break;
        case RBRACE:        kindString = "RBRACE";      break;
        case LBRACK:        kindString = "LBRACK";      break;
        case RBRACK:        kindString = "RBRACK";      break;
        case BECOMES:       kindString = "BECOMES";     break;
        case EQ:            kindString = "EQ";          break;
        case NE:            kindString = "NE";          break;
        case LT:            kindString = "LT";          break;
        case GT:            kindString = "GT";          break;
        case LE:            kindString = "LE";          break;
        case GE:            kindString = "GE";          break;
        case PLUS:          kindString = "PLUS";        break;
        case MINUS:         kindString = "MINUS";       break;
        case STAR:          kindString = "STAR";        break;
        case SLASH:         kindString = "SLASH";       break;
        case PCT:           kindString = "PCT";         break;
        case AMP:           kindString = "AMP";         break;
        case AND:           kindString = "AND";         break;
        case COMMA:         kindString = "COMMA";       break;
        case SEMI:          kindString = "SEMI";        break;
        case COMMENT:       kindString = "COMMENT";     break;
        case WHITESPACE:    kindString = "WHITESPACE";  break;
        case NUL:           kindString = "NUL";         break;

        // Non-terminal
        case Start:
            kindString = "Start";
            break;
        case bof:
            kindString = "bof";
            break;
        case eof:
            kindString = "eof";
            break;
        case proceduresAll:
            kindString = "proceduresAll";
            break;
        case procedures:
            kindString = "procedures";
            break;
        case procedureWain:
            kindString = "procedureWain";
            break;
        case procedure:
            kindString = "procedure";
            break;
        case type:
            kindString = "type";
            break;
        case dcls:
            kindString = "dcls";
            break;
        case dcl:
            kindString = "dcl";
            break;
        case statements:
            kindString = "statements";
            break;
        case statement:
            kindString = "statement";
            break;
        case tests:
            kindString = "tests";
            break;
        case test:
            kindString = "test";
            break;
        case expr:
            kindString = "expr";
            break;
        case term:
            kindString = "term";
            break;
        case factor:
            kindString = "factor";
            break;
        case lvalue:
            kindString = "lvalue";
            break;
    }

    return kindString;
}

int Token::toInt()
{
    int integer = 0;
    if (this->kind == INT) integer = std::atoi(this->lexeme.c_str());
    else throw NOT_AN_INT;
    return integer;
}
//

std::string toString(unsigned num)
{
    std::stringstream ss;
    ss << num;
    return ss.str();
}

}
