#include <iostream>
#include <string>
#include <functional>
#include <filesystem>
#include <map>

#include "Node.h"

namespace fs = std::filesystem;

// Compile file, make entity usable in other files using components
// file.vhdl -> file.o ghdl -a <file path> 

// Link and Gen binary from analysed entities
// file.o -> executable: ghdl -e <entity name>

int main() {
    // Iterate over all vhdl files
    std::unordered_map<std::string, std::shared_ptr<vm::Node>> dag;

    fs::directory_iterator working_dir (std::filesystem::current_path());
    for(const auto& file : working_dir) {
        if(file.path().extension() != ".vhdl") {
            continue;
        }

        auto node = std::make_shared<vm::Node>(file.path());

        for(const auto& entity : node->get_entitiy_definitions()) {     
            dag.insert({entity, node});            
        }
    }


    // Finish dependency graph by adding all the dependants to the correct nodes
    for(const auto& node : dag) {
        for(const auto& dependency : node.second->get_component_definitions()) {
            dag[dependency]->add_dependant(node.second);
        }
    }

    for(const auto& node : dag) {
        std::cout << "Node: " << node.first << std::endl;
        
        for(const auto& dependant : node.second->get_dependants()) {
            std::cout << "  " << dependant->get_file_path() << std::endl;
        }
    }
}  