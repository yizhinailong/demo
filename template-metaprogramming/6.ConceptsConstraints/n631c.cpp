import std;

namespace n631c {
    template <typename T>
    struct wrapper
    {
        T value;

        wrapper(T const& v) 
            requires std::is_copy_constructible_v<T> 
        :value(v) {}
    };
} // namespace n631c

auto main() -> int {
    {
        using namespace n631c;
        
        wrapper<int> a = 42;
        
        //wrapper<std::unique_ptr<int>> p = std::make_unique<int>(42); //error
    }
    return 0;
}
