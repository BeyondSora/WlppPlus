/******************************************************************************
 * This namespace contains what is shared across
 *  context-free and context-sensitive parsing.
 *
 * More implementation details are placed in .cc file,
 *  but end users should have no need for those.
 ******************************************************************************/
#ifndef PARSE_COMMON_H
#define PARSE_COMMON_H

#include <string>
#include <vector>

namespace parse_common {

enum DerivationRule {
};

class tree {
    public:
        tree();
        ~tree();
        std::string getRule();
        tree* getLeaf(int i);
        std::string getID();    // Only terminal symbols have identifiers

    private:
        DerivationRule rule;
        std::vector<std::string> units;
        std::vector<tree*> leaves;
};

class treePtr {
};

}

#endif//PARSE_COMMON_H
