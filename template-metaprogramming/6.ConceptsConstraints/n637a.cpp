import std;

namespace n637a {
    template <typename T>
    concept addable = requires(T a, T b) { a + b; }; // requires expression

    template <typename T>
        requires addable<T>  // requires clause with concept
    auto add(T a, T b)
    {
        return a + b;
    }
} // namespace n637a

auto main() -> int {
    return 0;
}
