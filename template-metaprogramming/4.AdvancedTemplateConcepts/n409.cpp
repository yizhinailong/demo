import std;

namespace n409 {
    template <unsigned int N>
    inline constexpr unsigned int factorial = N * factorial<N - 1>;

    template <>
    inline constexpr unsigned int factorial<0> = 1;
} // namespace n409

auto main() -> int {
    {
        using namespace n409;

        std::cout << factorial<0> << '\n';
        std::cout << factorial<1> << '\n';
        std::cout << factorial<2> << '\n';
        std::cout << factorial<3> << '\n';
        std::cout << factorial<4> << '\n';
        std::cout << factorial<5> << '\n';
        std::cout << factorial<12> << '\n';
    }
    return 0;
}
