#include <iostream>

#include "Builder.hpp"

#define VHDLMAKE_VERSION "0.1.1"

static void help() {
    std::cout << "List of commands:" << std::endl;
    std::cout << "vhdlmake build [entity] - builds entire project and optionaly links everything into <entity>" << std::endl;
    std::cout << "vhdlmake run <entity> - builds entire project and runs <entity>"  << std::endl;
}


int main(int argc, char *argv[]) {
    std::cout << "vhdlmake v" << VHDLMAKE_VERSION;

    #ifdef DEBUG
        std::cout << " DEBUG BUILD " << std::endl << std::endl;;
    #else
        std::cout << " RELEASE BUILD " << std::endl << std::endl;;
    #endif

    if(argc < 2) {
        help();
        return EXIT_FAILURE;
    }

    std::string command = argv[1];
    std::string entity;

    if(argc > 2) {
        entity = argv[2];
    }


    vm::Builder builder;

    if(command == "build") {
        builder.prepare();
        builder.build(entity);
    } else if (command == "run") {
        if(argc != 3) {
            std::cout << "Please provide an entity to run" << std::endl;
            return EXIT_FAILURE;
        }

        builder.build(entity);
        builder.run(entity);
    } else if(command == "clean") {
        builder.clean();
    } else {
        help();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}  