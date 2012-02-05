#include "scan.h"


#define WHITESPACES     "\t\n\r "
#define LETTERS         "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#define DIGITS          "0123456789"
#define ONE_TO_NINE     "123456789"

namespace scan {

    enum State {
        ST_NUL,         // NULL state
        ST_START,       // START state

        ST_ID,          // ID
                        // Identifiers are temporarily in
                        //  this state during tokenization.

        ST_INT,         // INT
        ST_ZERO,        // 0

        ST_LPAREN,      // (
        ST_RPAREN,      // )
        ST_LBRACE,      // {
        ST_RBRACE,      // }
        ST_LBRACK,      // [
        ST_RBRACK,      // ]

        ST_BECOMES,     // =
        ST_EQ,          // ==
        ST_EX,          // !
        ST_NE,          // !=
        ST_LT,          // <
        ST_GT,          // >
        ST_LE,          // <=
        ST_GE,          // >=

        ST_PLUS,        // +
        ST_MINUS,       // -
        ST_STAR,        // *
        ST_SLASH,       // /
        ST_PCT,         // %
        ST_AMP,         // &

        ST_AND,         // &&

        ST_COMMA,       // ,
        ST_SEMI,        // ;

        ST_COMMENT,     // //
        ST_WHITESPACE,  // Whitespace
    };

    static common::Kind const stateKinds[] = {
        common::NUL,            // NULL state
        common::NUL,            // START state

        common::ID,             // ID
                                // Identifiers are temporarily in
                                //  this state during tokenization.

        common::INT,            // INT
        common::INT,            // 0

        common::LPAREN,         // (
        common::RPAREN,         // )
        common::LBRACE,         // {
        common::RBRACE,         // }
        common::LBRACK,         // [
        common::RBRACK,         // ]

        common::BECOMES,        // =
        common::EQ,             // ==
        common::NUL,            // !
        common::NE,             // !=
        common::LT,             // <
        common::GT,             // >
        common::LE,             // <=
        common::GE,             // >=

        common::PLUS,           // +
        common::MINUS,          // -
        common::STAR,           // *
        common::SLASH,          // /
        common::PCT,            // %
        common::AMP,            // &

        common::AND,            // &&

        common::COMMA,          // ,
        common::SEMI,           // ;

        common::COMMENT,        // //
        common::WHITESPACE,     // Whitespace
    };

    static State delta[ST_WHITESPACE+1][256];

    static bool initT_done = false;

    // Initialize Transition Table for the compiler on first call.
    void initT();
    void setT(State from, std::string chars, State to);

    // Tokenize one line of source code.
    common::TokenLine lineTokenize(std::string const& line);

    common::Tokens tokenize(common::Lines const& lines)
    {
        common::Tokens tokens;
        common::TokenLine tokenLine;

        for (unsigned i = 0; i < lines.size(); ++i) {
            tokenLine = lineTokenize(lines[i]);
            tokens.push_back(tokenLine);
        }

        return tokens;
    }

    void initT() {
        // The default transition is ST_NUL
        //  (i.e., no transition defined for this char).
        for (unsigned i = 0; i <= ST_WHITESPACE; ++i) {
            for (unsigned j = 0; j < 256; j++) {
                delta[i][j] = ST_NUL;
            }
        }

        // Non-null transitions of the finite state machine.
        // NB: in the third line below, letters digits are macros
        //  that are replaced by string literals, which the compiler
        //  will concatenate into a single string literal.
        setT ( ST_START,      WHITESPACES,    ST_WHITESPACE );
        setT ( ST_WHITESPACE, WHITESPACES,    ST_WHITESPACE );
        setT ( ST_START,      LETTERS,        ST_ID         );
        setT ( ST_ID,         LETTERS DIGITS, ST_ID         );
        setT ( ST_START,      "0",            ST_ZERO       );
        setT ( ST_START,      ONE_TO_NINE,    ST_INT        );
        setT ( ST_INT,        DIGITS,         ST_INT        );
        setT ( ST_START,      "-",            ST_MINUS      );
        setT ( ST_START,      "+",            ST_PLUS       );
        setT ( ST_START,      "*",            ST_STAR       );
        setT ( ST_START,      "/",            ST_SLASH      );
        setT ( ST_SLASH,      "/",            ST_COMMENT    );
        setT ( ST_START,      "%",            ST_PCT        );
        setT ( ST_START,      ";",            ST_SEMI       );
        setT ( ST_START,      ",",            ST_COMMA      );
        setT ( ST_START,      "(",            ST_LPAREN     );
        setT ( ST_START,      ")",            ST_RPAREN     );
        setT ( ST_START,      "{",            ST_LBRACE     );
        setT ( ST_START,      "}",            ST_RBRACE     );
        setT ( ST_START,      "[",            ST_LBRACK     );
        setT ( ST_START,      "]",            ST_RBRACK     );
        setT ( ST_START,      "&",            ST_AMP        );
        setT ( ST_AMP,        "&",            ST_AND        );
        setT ( ST_START,      "=",            ST_BECOMES    );
        setT ( ST_BECOMES,    "=",            ST_EQ         );
        setT ( ST_START,      "<",            ST_LT         );
        setT ( ST_LT,         "=",            ST_LE         );
        setT ( ST_START,      ">",            ST_GT         );
        setT ( ST_GT,         "=",            ST_GE         );
        setT ( ST_START,      "!",            ST_EX         );
        setT ( ST_EX,         "=",            ST_NE         );


        for (unsigned i = 0; i < 256; ++i) {
            delta[ST_COMMENT][i] = ST_COMMENT;
        }
    }

    void setT(State from, std::string chars, State to) {
        for (unsigned i = 0; i < chars.length(); ++i) {
            delta[from][(unsigned)chars[i]] = to;
        }
    }

    common::TokenLine lineTokenize(std::string const& line)
    {
        // Initialize the transition table when called for the first time.
        if (initT_done == false) {
            initT();
            initT_done = true;
        }

        common::TokenLine tokenLine;
        unsigned i = 0;
        unsigned startIndex = 0;
        State state = ST_START;

        if (line.length() > 0) {
            while (true) {
                State nextState = ST_NUL;

                if (i < line.length()) {
                    nextState = delta[state][(unsigned) line[i]];
                }

                if (nextState == ST_NUL) {
                    // No more transitions possible
                    if (stateKinds[state] == common::NUL) {
                        // NEED to be RE-implemented
                        throw "ERROR\n";
                    }

                    if (stateKinds[state] != common::WHITESPACE) {
                        common::Token token;
                        token.lexeme = line.substr(startIndex, i - startIndex);
                        token.kind = stateKinds[state];
                        tokenLine.push_back(token);
                    }

                    startIndex = i;
                    state = ST_START;

                    if (i >= line.length()) {
                        break;
                    }
                }
                else {
                    state = nextState;
                    ++i;
                }
            }
        }

        return tokenLine;
    }

}
