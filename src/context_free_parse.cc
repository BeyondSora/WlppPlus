#include "context_free_parse.h"

#include "lr1_rules.h"

namespace context_free_parse {

using namespace lr1_rules;  // LR1ParseRule is only in another file
                            //  because of its large number of code.

Tree* build_parse_tree(common::Tokens &tokens)
{
}

}
