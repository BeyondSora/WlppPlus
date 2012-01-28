/******************************************************************************
 * Everything shared across classes, namespaces and such is
 *  included in this common namespace.
 *
 * Include this header in every other class/namespace.
 ******************************************************************************/

#ifndef COMMON_H
#define COMMON_H

#include <assert.h>
#include <stdlib.h>

#include <string>
#include <vector>

namespace common {

    typedef std::vector<std::string> WordLine;
    typedef std::vector<WordLine> Words;

    // Each token has one of the following kinds
    //  corresponding to lexical syntax of Wlpp+
    enum Kind {
        ID,         // Any identifier that is not a reserved keyword

        WAIN,       // wain
        IF,         // if
        ELSE,       // else
        WHILE,      // while
        RETURN,     // return

        INT,        // Integers

        LPAREN,     // (
        RPAREN,     // )
        LBRACE,     // {
        RBRACE,     // }
        LBRACK,     // [
        RBRACK,     // ]

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
        SEMI        // ;
    };

    // Atomic unit for Wlpp+
    struct Token {
        Kind kind;
        std::string lexeme;

        std::string getID();    // only for ID tokens
        int toInt();            // only for INT tokens
    };

    typedef std::vector<Token> TokenLine;
    typedef std::vector<TokenLine> Tokens;

    std::string toString(int num);

}

#endif
