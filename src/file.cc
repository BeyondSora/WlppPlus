#include "file.h"

#include "algorithm"
#include "iterator"
#include "sstream"

void file::err (Errorcode e, std::string msg)
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

common::Words file::toWords(std::string filename)
{
    common::Words words;

    try {
        std::ifstream ifs(filename.c_str());
        if (ifs.fail()) throw FILE_CANNOT_OPEN;

        toWords(ifs, words);

        ifs.close();
        if (ifs.fail()) throw FILE_CANNOT_CLOSE;
    }
    catch (Errorcode e) {
        err(e, filename);
    }

    return words;
}

typedef std::istream_iterator<std::string> Word;

void file::toWords(std::ifstream& ifs, common::Words& words)
{
    std::string line;

    while (std::getline(ifs, line)) {
        std::istringstream iss(line);
        common::WordLine wordLine;

        std::copy(Word(iss), Word(), std::back_inserter(wordLine));
        words.push_back(wordLine);
    }

    ifs.clear();    // clear error flag set by std::getline()
}
