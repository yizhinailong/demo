import std;

namespace n410 {
    constexpr unsigned int factorial(unsigned int const n) {
        return n > 1 ? n * factorial(n - 1) : 1;
    }

    template <typename T>
    struct wrapper {};

    template <int N>
    struct manyfold_wrapper {
        using value_type = wrapper<typename manyfold_wrapper<N - 1>::value_type>;
    };

    template <>
    struct manyfold_wrapper<0> {
        using value_type = unsigned int;
    };
} // namespace n410

auto main() -> int {
    {
        using namespace n410;

        std::cout << factorial(0) << '\n';
        std::cout << factorial(1) << '\n';
        std::cout << factorial(2) << '\n';
        std::cout << factorial(3) << '\n';
        std::cout << factorial(4) << '\n';
        std::cout << factorial(5) << '\n';

        std::cout << typeid(manyfold_wrapper<0>::value_type).name() << '\n';
        std::cout << typeid(manyfold_wrapper<1>::value_type).name() << '\n';
        std::cout << typeid(manyfold_wrapper<2>::value_type).name() << '\n';
        std::cout << typeid(manyfold_wrapper<3>::value_type).name() << '\n';
    }
    return 0;
}
