#include "Builder.hpp"
#include "DependencyGraph.hpp"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace vm {
    Builder::Builder() {
        m_BinaryDirectory = "bin";
        m_VcdDirectory = "vcd";
        m_CacheFile = ".vhdlmake"; 
        m_SourceDirectory = fs::current_path();
    }

    void Builder::prepare() {
        // Make sure buid directory exists
        if(!fs::exists(m_BinaryDirectory)) {
            fs::create_directory(m_BinaryDirectory);
        }

        if(!fs::exists(m_VcdDirectory)) {
            fs::create_directory(m_VcdDirectory);
        }
    }

    std::string Builder::cmd_compile(const std::string& file) {
        std::stringstream stream;
        stream << "ghdl -a " << file;
        return stream.str();
    }

    std::string Builder::cmd_link(const std::string& entity) {
        std::stringstream stream;
        stream << " ghdl -e " << entity;
        return stream.str();
    }

    std::string Builder::cmd_run(const std::string& entity) {
        std::stringstream stream;
        stream << "ghdl -r " << entity << " --vcd=" << m_VcdDirectory << "/" << entity << ".vcd";
        return stream.str();
    }

    void Builder::build(const std::string& entity) {
        DependencyGraph graph(m_SourceDirectory, m_CacheFile);
        std::vector<std::string> update_list = graph.get_update_list();

        // No need to build if no files were changed
        if(update_list.empty()) {
            std::cout << "No changes" << std::endl;
        }

        // Analyze all files
        for(const auto& unit : update_list) {
            std::cout << "[COMPILE] " << unit << std::endl;
            auto command = cmd_compile(unit);
            system(command.c_str());
        }

        // Link final entity if needed 
        if(entity != "") {
            std::cout << "[LINK] " << entity << std::endl;
            auto command = cmd_link(entity);
            system(command.c_str());

            // Move executable to bianry dir
            if(fs::exists(entity)) {
                fs::rename(entity, m_BinaryDirectory + "/" + entity);
            }
        }

        // Save hashes to cache
        graph.save_cache(m_CacheFile);
    }

    void Builder::run(const std::string& entity) {
        std::cout << "[RUN] " << entity << std::endl;
        auto command = cmd_run(entity);
        system(command.c_str());
    }

    void Builder::clean() {
        fs::recursive_directory_iterator working_dir (m_SourceDirectory);
        
        for(const auto& file : working_dir) {
            if(file.path().extension() == ".vcd") {
                fs::remove(file);
                std::cout << "[DELETE] " << file << std::endl;
            } else if(file.path().extension() == ".cf") {
                fs::remove(file);
                std::cout << "[DELETE] " << file << std::endl;
            }
        }

        std::cout << "Cleaned" << std::endl;
        prepare();
    }

} // namespace vm