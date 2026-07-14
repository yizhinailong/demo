import std;

namespace n508 {
    template <typename T, typename = typename std::enable_if_t<std::is_integral_v<T>>>
    struct integral_wrapper
    {
        T value;

        integral_wrapper(T v) : value(v) {}
    };

    template <typename T, typename = typename std::enable_if_t<std::is_floating_point_v<T>>>
    struct floating_wrapper
    {
        T value;

        floating_wrapper(T v) : value(v) {}
    };
} // namespace n508

auto main() -> int {
    {
        using namespace n508;
        
        integral_wrapper w1{ 42 };   // OK
        //integral_wrapper w2{ 42.0 }; // error
        //integral_wrapper w3{ "42" }; // error
        
        //floating_wrapper w4{ 42 };   // error
        floating_wrapper w5{ 42.0 }; // OK
        //floating_wrapper w6{ "42" }; // error
    }
    return 0;
}
