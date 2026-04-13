import std;

namespace n307 {
    template <typename T>
    auto sum(T a) -> T {
        return a;
    }

    template <typename T, typename... Args>
    auto sum(T a, Args... args) -> T {
        return a + sum(args...);
    }
} // namespace n307

auto main() -> int {
    {
        using namespace n307;

        std::println("sum(1) = {}", sum(1));
        std::println(
            "sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) = {}",
            sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
        );
    }
    return 0;
}
