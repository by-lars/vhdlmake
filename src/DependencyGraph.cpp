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
        build_dag(directory, cache_file);
    }

    void DependencyGraph::build_dag(const std::string& directory, const std::string& cache_file) {
        // Load cache
        json cache;
        if(fs::exists(cache_file)) {
            std::ifstream file (cache_file);
            file >> cache;
        }

        // Iterate over all vhdl files
        fs::recursive_directory_iterator working_dir (directory);
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

            // Generate Hash
            std::hash<std::string> hasher;
            size_t hash = hasher(buffer.str());

            // Add entities to lookup
            for(const auto& entity : data.Entities)  {
                this->entity_to_file[entity] = relative_path;
            } 

            // Add Node
            this->dag[relative_path] = Unit {
                .Path = relative_path,
                .Dependants = {},
                .Components = data.Components,
                .Hash = hash
            };

            // Add file to change list, if hashes don't match
            if(hash != cache[relative_path]) {
                std::cout << relative_path << " changed" << std::endl;
                this->changed_units.push_back(&this->dag[relative_path]);
            }
        }

        // Resolve dependants
        for(const auto& unit : dag) {
            for(const auto& dependency : unit.second.Components) {
                const std::string& dep_file_name = this->entity_to_file[dependency];
                dag[dep_file_name].Dependants.push_back((Unit*)&unit.second);
            }
        }
    }

    UnitData DependencyGraph::parse_unit(std::stringstream& unit) const {
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

    static void add_dependencies_recursive(std::vector<std::string>& list, Unit* current_unit, std::unordered_map<std::string, bool>& visited) {        
        for(Unit* unit : current_unit->Dependants) {
            if(visited[unit->Path]) {
                continue;
            }
            
            list.push_back(unit->Path);
            visited[unit->Path] = true;

            add_dependencies_recursive(list, unit, visited);
        }
    }

    std::vector<std::string> DependencyGraph::get_update_list() {
        std::vector<std::string> list;
        std::unordered_map<std::string, bool> visited;

        for(const auto& unit : changed_units) {
            if(visited[unit->Path]) {
                continue;
            }

            list.push_back(unit->Path);
            add_dependencies_recursive(list, unit, visited);
        }

        return list;
    }

    void DependencyGraph::save_cache(const std::string& cache_file) const {
        std::ofstream file(cache_file);

        if(!file.is_open()) {
            std::cerr << "Could not open cache file" << std::endl;
            return;
        }

        json data;
        for(const auto& unit : this->dag) {
            data[unit.first] = unit.second.Hash;
        }

        file << data;
    }

    void DependencyGraph::debug_print() const {
        for(const auto& node : this->dag) {
            std::cout << node.first << std::endl;
            std::cout << std::setw(4) << " path: "<< node.second.Path << std::endl;

            std::cout << std::setw(4) << "dependants: " << std::endl;
            for(const auto& dependant : node.second.Dependants) {
                std::cout << "  " << dependant->Path << std::endl;
            }

            std::cout << std::endl;
        }
    }
    
} // namespace vm
