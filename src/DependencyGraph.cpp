#include "DependencyGraph.hpp"

#include <filesystem>
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>

#include <fstream>

namespace fs = std::filesystem;
using json = nlohmann::json;

namespace vm {
    DependencyGraph::DependencyGraph(const std::string& directory, const std::string& cache_file) {
        build_dag(directory);
    }

    void DependencyGraph::build_dag(const std::string& directory) {
        // Iterate over all vhdl files
        fs::directory_iterator working_dir (directory);
        for(const auto& file_path : working_dir) {
            if(file_path.path().extension() != ".vhdl") {
                continue;
            }

            // Read file
            std::ifstream file(file_path.path());
            std::stringstream buffer;
            buffer << file.rdbuf();

            // Convert path to relative path
            std::string relative_path = fs::relative(file_path, directory).string();

            // Parse File
            UnitData data = parse_unit(buffer);  

            // Add Node
            this->dag[relative_path] = Unit {
                .Path = relative_path,
                .Dependants = {},
                .Entities = data.Entities,
                .Components = data.Components
            };
        }

        // Resolve dependants
        
    }

    UnitData DependencyGraph::parse_unit(std::stringstream& unit) {
        UnitData data;
        
        // Parse file
        std::string word;
        while (unit >> word) {
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            if (word == "entity") {
                std::string entity_name;
                unit >> entity_name;
                data.Entities.push_back(entity_name);
            } else if (word == "component") {
                std::string component_name;
                unit >> component_name;  
                data.Components.push_back(component_name);
            }
        }

        return data;
    }


    void DependencyGraph::unit_updated(const std::string& unit) {
        // ReParse unit for new entities, or new components
    }

    void DependencyGraph::unit_deleted(const std::string& unit) {
        // Delete unit from dag

    }

    void DependencyGraph::unit_added(const std::string& unit) {
        // Parse unit and add to dag
    }

    void DependencyGraph::unit_renamed(const std::string& old_name, const std::string& new_unit) {
        // ReName dag entry
    }

    void DependencyGraph::debug_print() const {
        for(const auto& node : this->dag) {
            std::cout << node.first << std::endl;
            std::cout << std::setw(4) << " path: "<< node.second.Path << std::endl;

            std::cout << std::setw(4) << "dependants: " << std::endl;
            for(const auto& dependant : node.second.Dependants) {
                std::cout << "  " << dependant << std::endl;
            }

            std::cout << std::endl;
        }
    }
    
} // namespace vm
