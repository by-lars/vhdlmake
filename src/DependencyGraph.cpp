#include "DependencyGraph.hpp"

#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>

namespace fs = std::filesystem;
using json = nlohmann::json;

namespace vm {
    DependencyGraph DependencyGraph::from_cache(const std::string& file) {
        DependencyGraph graph;
        return graph;
    }

    DependencyGraph DependencyGraph::build_partitial_dag(const std::string& directory) {
        DependencyGraph graph;
        return graph;
    }

    DependencyGraph DependencyGraph::build_complete_dag(const std::string& directory) {
        DependencyGraph graph;

        // Iterate over all vhdl files
        fs::directory_iterator working_dir (directory);
        for(const auto& file : working_dir) {
            if(file.path().extension() != ".vhdl") {
                continue;
            }

            auto node = std::make_shared<vm::Node>(
                fs::relative(file, directory).string()
            );

            for(const auto& entity : node->get_entitiy_definitions()) {     
                graph.m_Dag.insert({entity, node});            
            }
        }


        // Finish dependency graph by adding all the dependants to the correct nodes
        for(const auto& node : graph.m_Dag) {
            for(const auto& dependency : node.second->get_component_definitions()) {
                graph.m_Dag[dependency]->add_dependant({node.first, node.second});
            }
        }

        return graph;
    }

    void DependencyGraph::save_to_cache(const std::string& file) {
        json data;
        
        for(const auto& node : m_Dag) {
            data[node.first]["hash"] = node.second->get_hash();
            data[node.first]["path"] = node.second->get_file_path();
        
            for(const auto& dependant : node.second->get_dependants()) {
                data[node.first]["dependants"].push_back(dependant.first);
            }
        }

        std::ofstream out_file(file);
        out_file << std::setw(4) << data << std::endl;
    }
     

    void DependencyGraph::debug_print() {
        for(const auto& node : m_Dag) {
            std::cout << node.first << std::endl;
            std::cout << std::setw(4) << " hash: "<< node.second->get_hash() << std::endl;
            std::cout << std::setw(4) << " path: "<< node.second->get_file_path() << std::endl;
            std::cout << std::setw(4) << " entities: " << std::endl;

            for(const auto& entity : node.second->get_entitiy_definitions()) {
                std::cout << "  " << entity << std::endl;
            }

            std::cout << std::setw(4) << "dependants: " << std::endl;
            for(const auto& dependant : node.second->get_dependants()) {
                std::cout << "  " << dependant.first << std::endl;
            }

            std::cout << std::endl;
        }
    }


    
} // namespace vm
