import std;

namespace n408 {
    template <unsigned int N>
    struct factorial {
        static constexpr unsigned int value = N * factorial<N - 1>::value;
    };

    template <>
    struct factorial<0> {
        static constexpr unsigned int value = 1;
    };
} // namespace n408

auto main() -> int {
    {
        using namespace n408;

        std::cout << factorial<0>::value << '\n';
        std::cout << factorial<1>::value << '\n';
        std::cout << factorial<2>::value << '\n';
        std::cout << factorial<3>::value << '\n';
        std::cout << factorial<4>::value << '\n';
        std::cout << factorial<5>::value << '\n';
        std::cout << factorial<12>::value << '\n';
    }
    return 0;
}
