#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <sstream>

using std::string;
using std::stringstream;

namespace file {

    enum Errorcode {
        FILE_CANNOT_OPEN,
        FILE_CANNOT_CLOSE
    };

    void err (Errorcode e, string msg);

    // Since stringstream is not copy-able, must pass
    //  a function to obtain stringstream of the file
    void to_sstream(string filename, stringstream& src);

}

#endif
