#include "options.h"

void Options::handle_input(int argc, char *argv[]) {
    opterr = false; // Let us handle all error output for command line options
    int choice;
    int index = 0;

    option long_options[] = {
        {"help",  no_argument, nullptr, 'h'},
        {"verbose", no_argument, nullptr, 'v'},
        {nullptr, 0, nullptr, '\0'},
    }; // long_options[]

    while ((choice = getopt_long(argc, argv, "hv", long_options, &index)) != -1) {
        switch (choice) {
        case 'h':
            print_help();
            exit(0);

        case 'v':
            verbose = true;
            break;

        default:
            error("Unknown option");
        } // switch ..choice
    }     // while
} // getMode()


void Options::print_help() {
    std::cout << "a helpful message!" << '\n';
}

void Options::error(std::string message) {
    std::cerr << message << '\n';
    exit(1);
}
