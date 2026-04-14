import std;

namespace n314 {
    template <typename... T>
    auto sum(T... t) {
        return (... + t);
    }

    template <typename... T>
    auto sum_from_zero(T... t) {
        return (0 + ... + t);
    }
} // namespace n314

auto main() -> int {
    {
        using namespace n314;

        // std::println("{}", sum()); // error
        std::println("sum(1) = {}", sum(1));
        std::println("sum(1, 2, 3) = {}", sum(1, 2, 3));

        std::println("sum_from_zero() = {}", sum_from_zero());
        std::println("sum_from_zero(1, 2, 3) = {}", sum_from_zero(1, 2, 3));
    }
    return 0;
}
