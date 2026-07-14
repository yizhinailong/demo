import std;

namespace n629 {
    template <typename T>
    concept Integral = std::is_integral_v<T>;

    template <Integral T>
    T add(T a, T b)
    {
        return a + b;
    }

    template <Integral T>
    requires (sizeof(T) == 4)
    T add(T a, T b)
    {
        return a + b;
    }
} // namespace n629

auto main() -> int {
    {
        using namespace n629;
        
        add((short)1, (short)2);
        add(1, 2);
    }
    return 0;
}
