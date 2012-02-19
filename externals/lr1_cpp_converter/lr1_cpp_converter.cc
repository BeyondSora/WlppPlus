// This is a helper program to convert LR1 parse rules to the
//  corresponding C++ code for initializing the rules in an arry.
//
// This program takes a LR1 file as its input from stdin,
//  and outputs to stdout.
//
// Different LR1 file probably has different size for the rules.
// So, please modify the const LR1_RULES_SIZE below
//  to match the number of rules of your LR1 file.

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

const string LR1_RULES_SIZE = "733";

int main()
{
    string line;
    bool isRule = false;
    while (getline(cin, line))
    {
        if (isRule) {
            stringstream lineStream(line);
            string curState,
                   sym,
                   type,
                   next;
            lineStream >> curState
                >> sym
                >> type
                >> next;
            cout << "{ context_free_parse::"
                << (type == "reduce" ? "REDUCE" : "SHIFT") << ", "
                << curState << ", "
                << "common::" << sym << ", "
                << next <<  " },"
                << endl;
        }
        if (line == LR1_RULES_SIZE) isRule = true;
    }
    return 0;
}
