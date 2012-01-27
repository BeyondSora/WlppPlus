#include "common.h"

#include "file.h"
#include "scan.h"

int main()
{
    std::stringstream src;

    file::to_sstream("hello", src);

    common::toString(15);

    return 0;
}
