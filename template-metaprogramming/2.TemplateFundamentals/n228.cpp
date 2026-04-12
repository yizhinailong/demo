import std;

namespace n228 {
    template <typename T>
    struct is_floating_point {
        constexpr static bool value = false;
    };

    template <>
    struct is_floating_point<float> {
        constexpr static bool value = true;
    };

    template <>
    struct is_floating_point<double> {
        constexpr static bool value = true;
    };

    template <>
    struct is_floating_point<long double> {
        constexpr static bool value = true;
    };

    template <typename T>
    inline constexpr bool is_floating_point_v = is_floating_point<T>::value;
} // namespace n228

auto main() -> int {
    {
        using namespace n228;

        std::println("is_floating_point<int>::value = {}", is_floating_point<int>::value);
        std::println("is_floating_point<float>::value = {}", is_floating_point<float>::value);
        std::println("is_floating_point<double>::value = {}", is_floating_point<double>::value);
        std::println("is_floating_point<long double>::value = {}", is_floating_point<long double>::value);
        std::println("is_floating_point<std::string>::value = {}", is_floating_point<std::string>::value);
    }
    {
        using namespace n228;

        std::println("is_floating_point_v<int> = {}", is_floating_point_v<int>);
        std::println("is_floating_point_v<float> = {}", is_floating_point_v<float>);
        std::println("is_floating_point_v<double> = {}", is_floating_point_v<double>);
        std::println("is_floating_point_v<long double> = {}", is_floating_point_v<long double>);
        std::println("is_floating_point_v<std::string> = {}", is_floating_point_v<std::string>);
    }
    return 0;
}
