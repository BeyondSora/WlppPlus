/******************************************************************************
 * Everything shared across classes, namespaces and such is
 *  included in this common namespace.
 *
 * Include this header as the first one of local headers
 *  in every other class/namespace.
 ******************************************************************************/

#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>

namespace common {

typedef std::vector<std::string> Lines;

// Each token has one of the following kinds
//  corresponding to lexical syntax of Wlpp+.
enum Kind {
    // Terminal symbols
    ID,         // Any identifier that is not a reserved keyword

    WAIN,       // wain
    IF,         // if
    ELSE,       // else
    WHILE,      // while
    RETURN,     // return
    INTK,       // int
    CHARK,      // char

    INT,        // Integer
    CHAR,       // Character

    LPAREN,     // (
    RPAREN,     // )
    LBRACE,     // {
    RBRACE,     // }
    LBRACK,     // [
    RBRACK,     // ]

    BECOMES,    // =

    EQ,         // ==
    NE,         // !=
    LT,         // <
    GT,         // >
    LE,         // <=
    GE,         // >=

    PLUS,       // +
    MINUS,      // -
    STAR,       // *
    SLASH,      // /
    PCT,        // %
    AMP,        // &

    AND,        // &&

    COMMA,      // ,
    SEMI,       // ;

    COMMENT,    // //
    WHITESPACE, // Whitespace

    NUL,        // Bad/invalid token

    bof,
    eof,

    // Non-terminal symbols
    Start,
    proceduresAll,
    procedures,
    procedureWain,
    procedure,
    type,
    dcls,
    dcl,
    statements,
    statement,
    tests,
    test,
    expr,
    term,
    factor,
    lvalue,
};

// Atomic unit for Wlpp+
struct Token {
    Kind kind;
    std::string lexeme;

    std::string getKind();
    int toInt();            // Only for INT tokens
};

typedef std::vector<Token> TokenLine;
typedef std::vector<TokenLine> Tokens;

std::string toString(int num);

}

#endif//COMMON_H
