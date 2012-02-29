/******************************************************************************
 * This namespace performs semantic analysis.
 * Various forms of type-check's are performed.
 *
 * More implementation details are placed in source file,
 *  but end users should have no need for those.
 ******************************************************************************/
#ifndef SEMANTIC_PARSE_H
#define SEMANTIC_PARSE_H

#include "common.h"
#include "parse_common.h"

namespace semantic_parse {

using namespace parse_common;   // Use parse_common here,
                                //  because it is like a parent namespace.

struct VectorTree {
    ProductionRule rule;
    common::Token token;
    std::vector<VectorTree> leaves;
};

class ParseTree: public ParseTreeInterface {
    public:
        explicit ParseTree(Tree* tree);
    private:
        VectorTree vecTree_;
};

}

#endif//SEMANTIC_PARSE_H
