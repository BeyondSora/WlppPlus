#include "parse_common.h"

namespace parse_common {

Tree::Tree() {}

Tree::~Tree()
{
    for (unsigned i = 0; i < this->leaves.size(); ++i)
        delete this->leaves[i];
}

inline Tree* Tree::getLeaf(int i)
{
    return this->leaves.at(i);
}

inline std::string Tree::getID()
{
    std::string id;
    switch (this->rule) {
        default: throw "Not a terminal symbol\n"; break;

        case Ftor_Exp_Id:       // fall-through
        case Ftor_Exp_Int:      // fall-through
        case Ftor_Exp_Char:     // fall-through
        case Lval_Exp_Id:
                 id = this->units[1];
                 break;
    }
    return id;
}

}
