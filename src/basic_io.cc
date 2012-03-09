/******************************************************************************
 * Copyright (C) 2012 Jimmy Lu
 ******************************************************************************/

#include "basic_io.h"

namespace basic_io {

void out(std::string const& msg, Flag flag) {
    switch (flag) {
        default:
            throw "error";
            break;
        case ERROR: // fall-through
        case WARN:
            std::cerr << msg;
            break;
        case INFO:
            std::cout << msg;
            break;
    }
}

}
