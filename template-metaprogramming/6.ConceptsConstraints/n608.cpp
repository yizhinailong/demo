import std;

namespace n608 {
    template <typename T>
        requires std::is_arithmetic_v<T>
    T add(T const a, T const b)
    {
        return a + b;
    }

    template <typename T>
        requires std::is_arithmetic_v<T>
    T mul(T const a, T const b)
    {
        return a * b;
    }
} // namespace n608

auto main() -> int {
    return 0;
}
