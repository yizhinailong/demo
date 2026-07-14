import std;

namespace n510 {
    template <unsigned int n>
    constexpr unsigned int factorial()
    {
        if constexpr (n > 1)
            return n * factorial<n - 1>();
        else
            return 1;
    }
} // namespace n510

auto main() -> int {
    {
        std::cout << n510::factorial<1>() << '\n';
        std::cout << n510::factorial<2>() << '\n';
        std::cout << n510::factorial<3>() << '\n';
        std::cout << n510::factorial<4>() << '\n';
        std::cout << n510::factorial<5>() << '\n';
    }
    return 0;
}
