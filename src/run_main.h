/******************************************************************************
 * Copyright (C) 2012 Jimmy Lu
 *
 * This run_main class is where the compiler starts.
 ******************************************************************************/

#ifndef RUN_MAIN_H
#define RUN_MAIN_H

#include <string>
#include <vector>

class run_main {
    public:
        run_main(int argc, char **argv);
        ~run_main();
        // Compiler starts here.
        void run();
    private:
        std::vector<std::string> flags_;
};

#endif//RUN_MAIN_H
