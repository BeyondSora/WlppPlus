#include "parse_common.h"

namespace parse_common {

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

}
