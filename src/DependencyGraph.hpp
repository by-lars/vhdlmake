#include <string>
#include <unordered_map>
#include <memory>

#include "Node.hpp"

namespace vm {
    struct Unit {
        std::string Path;
        std::vector<std::string> Dependants;
        std::vector<std::string> Entities;
        std::vector<std::string> Components;
    };

    struct UnitData {
        std::vector<std::string> Entities;
        std::vector<std::string> Components;
    };

    class DependencyGraph {
    public:
        DependencyGraph(const std::string& directory, const std::string& cache_file);

        void unit_updated(const std::string& unit);
        void unit_deleted(const std::string& unit);
        void unit_added(const std::string& unit);
        void unit_renamed(const std::string& old_name, const std::string& new_unit);

        void debug_print() const;
    private:
        void build_dag(const std::string& directory);

        UnitData parse_unit(std::stringstream& unit);

        std::unordered_map<std::string, Unit> dag;
        std::unordered_map<std::string, std::string> entity_to_file;
    };
} // namespace vm
