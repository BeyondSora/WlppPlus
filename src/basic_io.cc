/******************************************************************************
 * Copyright (C) 2012 Jimmy Lu
 ******************************************************************************/

#include "basic_io.h"

namespace basic_io {

void out(std::string const& msg, Basic_io_flags flag) {
    switch (flag) {
        default:
            throw "error";
            break;
        case ERROR:
            std::cerr << msg;
            break;
        case INFO:
            std::cout << msg;
            break;
    }
}

}
