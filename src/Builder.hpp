#include <string>
#include <vector>

namespace vm {
    class Builder {
    public: 
        Builder();

        void prepare();
        void build();

    private:
        std::string m_BuildDirectory;
        std::string m_CacheFile;
        std::string m_SourceDirectory;

    };


} // namespace vm