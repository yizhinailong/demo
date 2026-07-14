import std;

template <class T>
constexpr bool always_false = std::false_type::value;

namespace n515 {
    template <typename T>
    std::string as_string(T value)
    {
        if constexpr (std::is_null_pointer_v<T>)
            return "null";
        else if constexpr (std::is_same_v<T, bool>)
            return value ? "true" : "false";
        else if constexpr (std::is_arithmetic_v<T>)
            return std::to_string(value);
        else
            static_assert(always_false<T>);
    }
} // namespace n515

auto main() -> int {
    {
        using namespace n515;
        std::cout << as_string(nullptr) << '\n';
        std::cout << as_string(true) << '\n';
        std::cout << as_string('a') << '\n';
        std::cout << as_string(42) << '\n';
        std::cout << as_string(42.0) << '\n';
    }
    return 0;
}
