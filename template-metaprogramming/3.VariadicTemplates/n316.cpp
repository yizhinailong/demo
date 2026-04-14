import std;

namespace n316 {
    template <int... R>
    constexpr int Sum = (... + R);

    template <int... I>
    constexpr auto indexes = std::make_index_sequence<5>();
} // namespace n316

auto main() -> int {
    {
        using namespace n316;

        std::println("Sum<1> = {}", Sum<1>);
        std::println("Sum<1, 2> = {}", Sum<1, 2>);
        std::println("Sum<1, 2, 3> = {}", Sum<1, 2, 3>);
    }
    return 0;
}
