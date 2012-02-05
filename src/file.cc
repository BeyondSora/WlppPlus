#include "file.h"

#include <cstdlib>
#include <fstream>
#include <iostream>

namespace file {

enum Errorcode {
    FILE_CANNOT_OPEN,
    FILE_CANNOT_CLOSE,
};

void err(Errorcode e, std::string msg);
void toLines(std::ifstream& ifs, common::Lines& lines);

common::Lines toLines(std::string const& filename)
{
    std::vector<std::string> lines;

    try {
        std::ifstream ifs(filename.c_str());
        if (ifs.fail()) throw FILE_CANNOT_OPEN;

        toLines(ifs, lines);

        ifs.close();
        if (ifs.fail()) throw FILE_CANNOT_CLOSE;
    }
    catch (Errorcode e) {
        err(e, filename);
    }

    return lines;
}

void err(Errorcode e, std::string msg)
{
    switch (e) {
        default:
            std::cerr << "Unknown error.\n";
            break;
        case FILE_CANNOT_OPEN:
            std::cerr << "file \"" << msg
                << "\" could not be opened.\n";
            break;
        case FILE_CANNOT_CLOSE:
            std::cerr << "file \"" << msg
                << "\" could not be closed.\n";
            break;
    }
    std::exit(-1);
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
