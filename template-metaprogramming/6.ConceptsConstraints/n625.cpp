import std;

namespace n625 {
    template <typename T>
    T add(T a, T b)
    {
        return a + b;
    }

    template <typename T>
    requires std::is_integral_v<T>
    T add(T a, T b)
    {
        return a + b;
    }
} // namespace n625

auto main() -> int {
    {
        using namespace n625;
        
        add(1.0, 2.0);
        add(1, 2);
    }
    return 0;
}
