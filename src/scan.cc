#include "scan.h"

#include <iostream>

#define WHITESPACES     "\t\n\r "
#define LETTERS         "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#define DIGITS          "0123456789"
#define ONE_TO_NINE     "123456789"

enum State {
};

common::Kind stateKinds[] = {
};

common::Tokens scan::operator() (const common::Lines &lines)
{
    common::TokenLine tokenLine;
    std::string line;

    return this->tokens;
}
