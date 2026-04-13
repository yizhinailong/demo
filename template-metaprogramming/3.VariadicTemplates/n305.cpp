import std;

namespace n305 {
    template <typename T>
    auto min(T a, T b) -> T {
#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
        std::println("{}", __PRETTY_FUNCTION__);
#elif defined(_MSC_VER)
        std::println("{}", __FUNCSIG__);
#endif
        return a < b ? a : b;
    }

    template <typename T, typename... Args>
    T min(T a, Args... args) {
#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
        std::println("{}", __PRETTY_FUNCTION__);
#elif defined(_MSC_VER)
        std::println("{}", __FUNCSIG__);
#endif
        return min(a, min<T>(args...));
    }
} // namespace n305

auto main() -> int {
    {
        using namespace n305;

        min<double>(1, 5, 3, -4, 9.0);
    }
    return 0;
}
