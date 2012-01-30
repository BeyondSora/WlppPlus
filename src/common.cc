#include "common.h"

#include <assert.h>

namespace common {

    //
    // Token struct member functions
    std::string Token::getID()
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
    //

    std::string toString(int num)
    {
        // not implemented yet
        assert(false);
    }

}
