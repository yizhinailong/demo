import std;

namespace n636a {
    auto add(auto a, auto b)
    {
        return a + b;
    }

    template<>
    auto add(char const* a, char const* b)
    {
        return std::string(a) + std::string(b);
    }
} // namespace n636a

auto main() -> int {
    {
        using namespace n636a;
        
        add(4, 2);
        add(4.0, 2);
        add("4", "2");
    }
    return 0;
}
