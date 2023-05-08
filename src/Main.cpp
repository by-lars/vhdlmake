#include <iostream>

#define VHDLMAKE_VERSION "0.1.0"

// Compile file, make entity usable in other files using components
// file.vhdl -> file.o ghdl -a <file path> 

// Link and Gen binary from analysed entities
// file.o -> executable: ghdl -e <entity name>

static void version() {
    std::cout << "vhdlmake v" << VHDLMAKE_VERSION;

    #ifdef DEBUG 
        std::cout << " - DEBUG BUILD";
    #endif

    std::cout << std::endl;
}

static void help() {
    version();
    std::cout << std::endl;
    std::cout << "help - prints the help menu" << std::endl;
    std::cout << "version - prints the version of vhdlmake" << std::endl;
    std::cout << "run - run a target" << std::endl;
    std::cout << "build - build current directory" << std::endl;
}

static void build() {

}


static void run() {

}

static void unknown_command() {
    std::cout << "unknown command, enter \"vhdlmake help\" to get a full list of available commands" << std::endl;
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        help();
        return EXIT_FAILURE;
    }

    if(argv[1] == "run") {
        run();
    } else if(argv[1] == "build") {
        build();
    } else if (argv[1] == "help") {
        help();
    } else if (argv[1] == "version") {
        version();
    } else {
        unknown_command();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}  