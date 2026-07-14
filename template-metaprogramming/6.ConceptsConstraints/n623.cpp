import std;

namespace n623 {
    template <typename T>
    concept Integral = std::is_integral_v<T>;

    template <typename ... T>
    requires (Integral<T> && ...)
    auto add(T ... args)
    {
        return (args + ...);
    }
} // namespace n623

auto main() -> int {
    {
        using namespace n623;
        
        add(1, 2, 3);
        //add(1, 42.0);
    }
    return 0;
}
