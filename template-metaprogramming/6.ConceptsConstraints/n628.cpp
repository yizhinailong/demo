import std;

namespace n628 {
    template <typename T>
    concept Integral = std::is_integral_v<T>;

    template <typename T>
        requires Integral<T>
    T add(T a, T b)
    {
        return a + b;
    }

    template <typename T>
        requires Integral<T> && (sizeof(T) == 4)
    T add(T a, T b)
    {
        return a + b;
    }
} // namespace n628

auto main() -> int {
    {
        using namespace n628;
        
        add((short)1, (short)2);
        add(1, 2);
    }
    return 0;
}
