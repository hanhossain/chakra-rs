#include <memory>
namespace simple {
    class Simple {
    public:
        Simple();
        void hello_world() const;
    };
    
    std::unique_ptr<Simple> Simple_new();
}