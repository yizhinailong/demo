import std;

namespace n619 {
    template <typename T>
    requires std::is_integral_v<T> || std::is_floating_point_v<T>
    T add(T a, T b)
    {
        return a + b;
    }
} // namespace n619

auto main() -> int {
    return 0;
}
