#include <string>
#include <unordered_map>

#include "Node.hpp"

namespace vm {
    class DependencyGraph {
    public:
        static DependencyGraph from_cache(const std::string& file);
        static DependencyGraph build_partitial_dag(const std::string& directory);
        static DependencyGraph build_complete_dag(const std::string& directory);

        void save_to_cache(const std::string& file);
        void debug_print();

    private:
        DependencyGraph() = default;

        std::unordered_map<std::string, std::shared_ptr<vm::Node>> m_Dag;
    };
} // namespace vm
