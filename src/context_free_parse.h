/******************************************************************************
 * This namespace performs context-free grammar analysis.
 * The tokenized source code is examined for the validity of
 *  its context-free grammar.
 *
 * More implementation details are placed in .cc file,
 *  but end users should have no need for those.
 ******************************************************************************/
#ifndef CONTEXT_FREE_PARSE_H
#define CONTEXT_FREE_PARSE_H

#include "common.h"
#include "parse_common.h"

namespace context_free_parse {

void build_parse_tree();

}

#endif//CONTEXT_FREE_PARSE_H
