import std;

namespace n631a {
    template <typename T>
    struct wrapper
    {
        T value;

        wrapper(T const & v) :value(v) {}
    };
} // namespace n631a

auto main() -> int {
    {
        using namespace n631a;
        
        wrapper<int> a = 42;
        
        //wrapper<std::unique_ptr<int>> p = std::make_unique<int>(42); // error
    }
    return 0;
}
