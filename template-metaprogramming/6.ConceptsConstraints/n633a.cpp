import std;

namespace n633a {
    template <std::integral T>
    struct wrapper
    {
        T value;
    };
} // namespace n633a

auto main() -> int {
    {
        using namespace n633a;
        
        wrapper<int> a{ 42 };
        // wrapper<double> b{ 42.0 }; // error
    }
    return 0;
}
