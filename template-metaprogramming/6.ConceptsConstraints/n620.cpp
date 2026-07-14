import std;

namespace n620 {
    template <typename T>
    concept Integral = std::is_integral_v<T>;

    template <typename T>
    concept FloatingPoint = std::is_floating_point_v<T>;

    template <typename T>
    concept Number = Integral<T> || FloatingPoint<T>;

    template <Number T>
    T add(T a, T b)
    {
        return a + b;
    }
} // namespace n620

auto main() -> int {
    return 0;
}
