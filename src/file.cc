#include "file.h"

#include <cstdlib>

void file::err(Errorcode e, std::string msg)
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

common::Lines file::toLines(std::string filename)
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

void file::toLines(std::ifstream& ifs, common::Lines &lines)
{
    std::string line;

    while(std::getline(ifs, line)) {
        lines.push_back(line);
    }

    ifs.clear();    // clear error bit set by getline()
}
