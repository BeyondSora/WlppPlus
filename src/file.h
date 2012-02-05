/******************************************************************************
 * This namespace is for opening/closing and modifying files.
 *
 * More implementation details are placed in .cc file,
 *  but end users should have no need for those.
 ******************************************************************************/

#ifndef FILE_H
#define FILE_H

#include "common.h"

namespace file {

    common::Lines toLines(std::string const& filename);

}

#endif
