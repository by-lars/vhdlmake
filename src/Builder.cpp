#include "Builder.hpp"
#include "DependencyGraph.hpp"

#include <filesystem>
#include <iostream>


namespace fs = std::filesystem;

namespace vm {

    Builder::Builder() {
        m_BuildDirectory = "build";
        m_CacheFile = m_BuildDirectory + "/vhdlmake.cache"; 
        m_SourceDirectory = fs::current_path();
    }

    void Builder::prepare() {
        // Make sure buid directory exists
        if(!fs::exists(m_BuildDirectory)) {
            fs::create_directory(m_BuildDirectory);
        }
    }

    void Builder::build() {
        DependencyGraph dg(m_SourceDirectory, m_CacheFile);
        dg.debug_print();
    }

} // namespace vm