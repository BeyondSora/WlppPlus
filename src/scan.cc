#include "scan.h"

#include "common.h"

string Token::getID()
{
    assert(this->kind == ID);

    return "not implemented yet\n";
}

int Token::toInt()
{
    assert(this->kind == INT);

    //not implemented yet
    return 0;
}

scan::scan() {};

scan::~scan() {};

Tokens scan::operator() (stringstream src)
{
}
