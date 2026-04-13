import std;

namespace n306 {
    template <typename T, typename... Args>
    auto sum(T a, Args... args) -> T {
        if constexpr (sizeof...(args) == 0) {
            return a;
        } else {
            return a + sum(args...);
        }
    }
} // namespace n306

auto main() -> int {
    {
        using namespace n306;

        std::println("sum(1) = {}", sum(1));
        std::println(
            "sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) = {}",
            sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
        );
    }
    return 0;
}
