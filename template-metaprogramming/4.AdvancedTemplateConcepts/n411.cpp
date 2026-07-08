import std;

namespace n411 {
    template <unsigned int N>
    inline constexpr unsigned int sum = N + sum<N - 1>;

    template <>
    inline constexpr unsigned int sum<0> = 0;
} // namespace n411

auto main() -> int {
    {
        using namespace n411;

        std::cout << sum<256> << '\n';
    }
    return 0;
}
