/******************************************************************************
 * Copyright (C) 2012 Jimmy Lu
 ******************************************************************************/

#include "file.h"

#include <cstdlib>
#include <fstream>
#include <iostream>

#include "error.h"

namespace file {

void toLines(std::ifstream& ifs, common::Lines& lines);

common::Lines toLines(std::string const& filename)
{
    std::vector<std::string> lines;

    std::ifstream ifs(filename.c_str());
    if (ifs.fail()) {
        throw new error::ErrorObject(error::FILE_CANNOT_OPEN,
                "file::toLines(std::string const&)",
                "could not open file: \"" + filename + "\".");
    }

    toLines(ifs, lines);

    ifs.close();
    if (ifs.fail()) {
        throw new error::ErrorObject(error::FILE_CANNOT_CLOSE,
                "file::toLines(std::string const&)",
                "could not open file: \"" + filename + "\".");
    }

    return lines;
}

void toLines(std::ifstream& ifs, common::Lines& lines)
{
    std::string line;

    while(std::getline(ifs, line)) {
        lines.push_back(line);
    }

    ifs.clear();    // Clear error bit set by getline().
}

}
