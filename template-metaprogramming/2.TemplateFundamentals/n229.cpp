import std;

namespace n229 {
    template <typename T>
    struct is_floating_point;

    template <>
    struct is_floating_point<float> {
        constexpr static bool value = true;
    };

    template <typename T>
    struct is_floating_point {
        constexpr static bool value = false;
    };
} // namespace n229

auto main() -> int {
    {
        using namespace n229;

        std::println("is_float_point<int>::value = {}", is_floating_point<int>::value);
        std::println("is_float_point<float>::value = {}", is_floating_point<float>::value);
    }
    return 0;
}
