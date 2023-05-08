#include "Builder.hpp"

#include <filesystem>

namespace fs = std::filesystem;

namespace vm {

    Builder::Builder() {
        m_BuildDirectory = "build";
    }

    void Builder::prepare() {
        // Make sure buid directory exists
        if(!fs::exists(m_BuildDirectory)) {
            fs::create_directory(m_BuildDirectory);
        }
    }

    void Builder::build() {

    }

} // namespace vm