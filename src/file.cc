#include "file.h"

#include <fstream>

#include "common.h"

using std::cerr;
using std::ifstream;
using std::string;
using std::stringstream;

namespace file {

    void err (Errorcode e, string msg)
    {
        switch (e) {
            default:
                cerr << "Unknown error.\n";
                break;
            case FILE_CANNOT_OPEN:
                cerr << "file " << msg
                    << " could not be opened.\n";
                break;
            case FILE_CANNOT_CLOSE:
                cerr << "file " << msg
                    << " could not be closed.\n";
                break;
        }
    }

    void to_sstream(string filename, stringstream& src)
    {
        try {
            ifstream src_file(filename.c_str());
            if (src_file.fail()) throw FILE_CANNOT_OPEN;

            src << src_file.rdbuf();

            src_file.close();
            if (src_file.fail()) throw FILE_CANNOT_CLOSE;
        }
        catch (Errorcode e) {
            err(e, filename);
        }
    }

}
