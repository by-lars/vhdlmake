#include "Builder.hpp"
#include "DependencyGraph.hpp"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace vm {
    Builder::Builder() {
        m_BuildDirectory = "build";
        m_BinaryDirectory = "bin";
        m_VcdDirectory = "vcd";
        m_CacheFile = m_BuildDirectory + "/vhdlmake.cache"; 
        m_SourceDirectory = fs::current_path();
    }

    void Builder::prepare() {
        // Make sure buid directory exists
        if(!fs::exists(m_BuildDirectory)) {
            fs::create_directory(m_BuildDirectory);
        }

        if(!fs::exists(m_BinaryDirectory)) {
            fs::create_directory(m_BinaryDirectory);
        }

        if(!fs::exists(m_VcdDirectory)) {
            fs::create_directory(m_VcdDirectory);
        }
    }

    std::string Builder::cmd_compile(const std::string& file) {
        std::stringstream stream;
        stream << "ghdl -a --workdir=" << m_BuildDirectory << " " << file;
        return stream.str();
    }

    std::string Builder::cmd_link(const std::string& entity) {
        std::stringstream stream;
        stream << "cd " << m_BuildDirectory << "; ghdl -e " << entity;
        return stream.str();
    }

    std::string Builder::cmd_run(const std::string& entity) {
        std::stringstream stream;
        stream << "cd " << m_BinaryDirectory <<  "; ghdl -r " << entity << " --vcd=../" << m_VcdDirectory << "/" << entity << ".vcd";
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
            auto file_path = m_BuildDirectory + "/" + entity;
            if(fs::exists(file_path)) {
                fs::rename(file_path, m_BinaryDirectory + "/" + entity);
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
        fs::remove_all(m_BinaryDirectory);
        fs::remove_all(m_BuildDirectory);
        fs::remove_all(m_VcdDirectory);
        prepare();
    }

} // namespace vm