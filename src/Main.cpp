#include <iostream>

#include "Builder.hpp"

#define VHDLMAKE_VERSION "0.1.0"

// Compile file, make entity usable in other files using components
// file.vhdl -> file.o ghdl -a <file path> 

// Link and Gen binary from analysed entities
// file.o -> executable: ghdl -e <entity name>

int main(int argc, char *argv[]) {
    vm::Builder builder;
    builder.prepare();
    builder.build();

    return EXIT_SUCCESS;
}  