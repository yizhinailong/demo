import std;

namespace n627 {
    template <typename T>
        requires std::is_integral_v<T>
    T add(T a, T b)
    {
        return a + b;
    }

    template <typename T>
        requires std::is_integral_v<T> && (sizeof(T) == 4)
    T add(T a, T b)
    {
        return a + b;
    }
} // namespace n627

auto main() -> int {
    {
        using namespace n627;
        
        add((short)1, (short)2);
        //add(1, 2);
    }
    return 0;
}
