import std;

namespace n208 {
    template <int V>
    class foo1 {};

    template <int = 1>
    class foo2 {};

    template <int... T>
    class foo3 {};
} // namespace n208

auto main() -> int {
    return 0;
}
