#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

namespace vm {
    struct Unit {
        std::string Path;
        std::vector<Unit*> Dependants;
        std::vector<std::string> Components;
        size_t Hash;
    };

    struct UnitData {
        std::vector<std::string> Entities;
        std::vector<std::string> Components;
    };


    class DependencyGraph {
    public:
        DependencyGraph(const std::string& directory, const std::string& cache_file);

        std::vector<std::string> get_update_list();

        void save_cache(const std::string& cache_file) const;
        void debug_print() const;

    private:
        void build_dag(const std::string& directory, const std::string& cache_file);
        UnitData parse_unit(std::stringstream& unit) const;

        std::unordered_map<std::string, Unit> dag;
        std::unordered_map<std::string, std::string> entity_to_file;
        std::vector<Unit*> changed_units;
    };
} // namespace vm
