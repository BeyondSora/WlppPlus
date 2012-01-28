/******************************************************************************
 * Do NOT initialize a new instance of scan class !!!
 *
 * An global static instance SCAN is already declared within this header.
 *  Please use that instead.
 ******************************************************************************/

#ifndef SCAN_H
#define SCAN_H

#include <vector>

#include "common.h"

// The only purpose of scan class is
//  to return Tokenized source code
class scan {

    public:
        scan();
        ~scan();

        // make scan a functor
        //  that takes source code in unit of words
        common::Tokens operator() (const common::Words& words);

    private:
        common::Tokens tokens;

};

static scan SCAN;

#endif
