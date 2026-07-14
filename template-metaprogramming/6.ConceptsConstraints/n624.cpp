import std;

namespace n624 {
    int add(int a, int b)
    {
        return a + b;
    }

    template <typename T>
    T add(T a, T b)
    {
        return a + b;
    }
} // namespace n624

auto main() -> int {
    {
        using namespace n624;
        
        add(1.0, 2.0);
        add(1, 2);
    }
    return 0;
}
