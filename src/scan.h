/******************************************************************************
 * This namespace is for scanning through the source file and tokenize it.
 *
 * More implementation details are placed in .cc file,
 *  but end users should have no need for those.
 ******************************************************************************/

#ifndef SCAN_H
#define SCAN_H

#include "common.h"

// The only purpose of scan namespace is
//  to return Tokenized source code.
namespace scan {

// Take source file in unit of words and tokenize it.
common::Tokens tokenize(common::Lines const& lines);

}

#endif
