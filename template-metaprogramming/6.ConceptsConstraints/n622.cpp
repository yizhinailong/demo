import std;

namespace n622 {
    template <typename ... T>
    requires (std::is_integral_v<T> && ...)
    auto add(T ... args)
    {
        return (args + ...);
    }
} // namespace n622

auto main() -> int {
    {
        using namespace n622;
        
        add(1, 2, 3);
        //add(1, 42.0);
    }
    return 0;
}
