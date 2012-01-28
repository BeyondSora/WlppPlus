/******************************************************************************
 * file class is only for opening and closing files
 *
 * All members are static
 *
 * Do NOT initialize an instance of this class !!!
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
        static common::Words toWords(std::string filename);

    private:
        static void err (Errorcode e, std::string msg);
        static void toWords(std::ifstream& ifs, common::Words& words);

};

#endif
