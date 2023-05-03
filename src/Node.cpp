#include "Node.h"

#include <iostream>
#include <fstream>
#include <string>

namespace vm
{
    Node::Node(const std::string &path)
        : m_FilePath(path) {
        std::ifstream file(path);

        // Make sure file is open
        if (file.is_open() == false)
        {
            std::cerr << "Could not find file: " << path << std::endl;
            throw std::runtime_error("Could not find file");
        }

        // Parse File for entity definitions or component definitions
        std::string word;
        while (file >> word) {
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);

            if (word == "entity") {
                std::string entity_name;
                file >> entity_name;
                m_EntityDefinitions.push_back(entity_name);
            } else if (word == "component") {
                std::string component_name;
                file >> component_name;
                m_ComponentDefinitions.push_back(component_name);
            }
        }
    }

    const std::string &Node::get_file_path() const {
        return m_FilePath;
    }

    const std::hash<std::string> &Node::get_hash() const {
        return m_Hash;
    }

    const std::vector<Node::pointer_t> &Node::get_dependants() const {
        return m_Dependants;
    }

    const std::vector<std::string> &Node::get_entitiy_definitions() const {
        return m_EntityDefinitions;
    }

    const std::vector<std::string> &Node::get_component_definitions() const {
        return m_ComponentDefinitions;
    }

    void Node::add_dependant(Node::pointer_t node) {
        m_Dependants.push_back(node);
    }
}