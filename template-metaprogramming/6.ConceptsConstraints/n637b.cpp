import std;

namespace n637b {
    template <typename T>
        requires requires(T a, T b) { a + b; }  // requires clause with requires expression
    auto add(T a, T b)
    {
        return a + b;
    }
} // namespace n637b

auto main() -> int {
    return 0;
}
