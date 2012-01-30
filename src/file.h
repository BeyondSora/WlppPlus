/******************************************************************************
 * This file class is only for opening and closing files.
 *
 * All members are static.
 *
 * Do NOT initialize an instance of this class !!!
 *  Treat this class as a namespace.
 ******************************************************************************/

#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <fstream>

#include "common.h"

enum Errorcode {
    FILE_CANNOT_OPEN,
    FILE_CANNOT_CLOSE
};

class file {

    public:
        static common::Lines toLines(std::string filename);

    private:
        static void err(Errorcode e, std::string msg);
        static void toLines(std::ifstream &ifs, common::Lines &lines);

};

#endif
