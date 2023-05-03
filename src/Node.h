#include <string>
#include <functional>
#include <vector>
#include <memory>

namespace vm {
    class Node {
    public:
        using pointer_t = std::shared_ptr<Node>;

        Node(const std::string& path);

        const std::string& get_file_path() const;
        const std::hash<std::string>& get_hash() const;
        const std::vector<Node::pointer_t>& get_dependants() const;
        const std::vector<std::string>& get_entitiy_definitions() const;
        const std::vector<std::string>& get_component_definitions() const;

        void add_dependant(Node::pointer_t node);

    private:
        std::string m_FilePath;
        std::hash<std::string> m_Hash;
        std::vector<std::string> m_EntityDefinitions;
        std::vector<std::string> m_ComponentDefinitions;
        std::vector<Node::pointer_t> m_Dependants;
    };

} // namespace vm 
