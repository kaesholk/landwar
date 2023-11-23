#pragma once

#include <string>
#include <stdint.h>
#include <getopt.h>
#include <iostream>
#include "error.h"

class Options {
    public:
        Options() : filename{""}, verbose{false} {}

        void handle_input(int argc, char * argv[]);
        bool get_verbose() {return verbose;}
    
    private:
        std::string filename;
        bool verbose;

        void print_help();

        void error(std::string message);
};