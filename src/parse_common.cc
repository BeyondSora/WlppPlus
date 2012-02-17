#include "parse_common.h"

namespace parse_common {

Tree::Tree() {}

Tree::~Tree()
{
    for (unsigned i = 0; i < this->leaves.size(); ++i)
        delete this->leaves[i];
}

std::string Tree::getRule()
{
    switch (this->rule) {
        default: throw "No such rule defined!\n"; break;
    }
}

}
