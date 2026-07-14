import std;

template <class T>
constexpr bool always_false = std::false_type::value;

namespace n517 {
    template <typename T>
    std::string as_string(T&& value)
    {
        using value_type = std::decay_t<T>;

        if constexpr (std::is_null_pointer_v<value_type>)
            return "null";
        else if constexpr (std::is_same_v<value_type, bool>)
            return value ? "true" : "false";
        else if constexpr (std::is_arithmetic_v<value_type>)
            return std::to_string(value);
        else
            static_assert(always_false<T>);
    }
} // namespace n517

auto main() -> int {
    {
        using namespace n517;
        std::cout << as_string(nullptr) << '\n';
        std::cout << as_string(true) << '\n';
        std::cout << as_string('a') << '\n';
        std::cout << as_string(42) << '\n';
        std::cout << as_string(42.0) << '\n';
        
        bool f = true;
        std::cout << as_string(f) << '\n';
        
        int n = 42;
        std::cout << as_string(n) << '\n';
    }
    return 0;
}
