import std;

namespace n303 {
    template <typename T>
    auto min(T a, T b) -> T {
        return a < b ? a : b;
    }

    template <typename T, typename... Args>
    auto min(T a, Args... args) -> T {
        return min(a, min<T>(args...));
    }
} // namespace n303

auto main() -> int {
    {
        using namespace n303;

        std::println("min<int>(2, 42, 7) = {}", min<int>(2, 42, 7));
        std::println("min<double>(5, 1, 5, 3, -4, 9.0) = {}", min<double>(5, 1, 5, 3, -4, 9.0));
    }
    return 0;
}
