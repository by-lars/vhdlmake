#include <string>

namespace vm {

    class Builder {
    public: 
        Builder();


        void prepare();

        void build();


    private:
        static std::string m_BuildDirectory;

    };


} // namespace vm