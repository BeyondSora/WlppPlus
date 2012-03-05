/******************************************************************************
 * Copyright (C) 2012 Jimmy Lu
 *
 * This namespace is intended to further abstract basic input/output to
 *  the operating system in an effort to make the compiler portable.
 *
 * Potentially, this could be merged with file namespace.
 *
 * For now, however, it is only intended for writing to stdin/stdout.
 ******************************************************************************/

#ifndef BASIC_IO_H
#define BASIC_IO_H

#include <iostream>
#include <string>

namespace basic_io {

enum Basic_io_flags {
    ERROR,
    INFO,
};

// Outputs msg to stdout/stderr depending on error flag.
// When not specified, flag defaults to INFO.
void out(std::string const& msg, Basic_io_flags flag = INFO);

}

#endif//BASIC_IO_H
