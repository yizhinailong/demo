import std;

namespace n409b {
    template <unsigned int n>
    constexpr unsigned int factorial() {
        return n * factorial<n - 1>();
    }

    template <>
    constexpr unsigned int factorial<1>() {
        return 1;
    }

    template <>
    constexpr unsigned int factorial<0>() {
        return 1;
    }
} // namespace n409b

auto main() -> int {
    {
        using namespace n409b;

        std::cout << factorial<1>() << '\n';
        std::cout << factorial<2>() << '\n';
        std::cout << factorial<3>() << '\n';
        std::cout << factorial<4>() << '\n';
        std::cout << factorial<5>() << '\n';
    }
    return 0;
}
