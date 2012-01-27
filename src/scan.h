#ifndef SCAN_H
#define SCAN_H

#include <sstream>
#include <vector>

using std::string;
using std::stringstream;
using std::vector;

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
    string lexeme;

    string getID();     // only for ID tokens
    int toInt();        // only for INT tokens
};

typedef vector<vector<Token> > Tokens;

class scan {

    public:
        scan();
        ~scan();

        Tokens operator() (stringstream src);   // make scan a functor

    private:
        static Kind kind;

};

#endif
