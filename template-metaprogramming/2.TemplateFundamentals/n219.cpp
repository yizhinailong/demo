import std;

namespace n219 {
    template <typename T, typename U = double>
    struct foo;

    template <typename T = int, typename U>
    struct foo;

    template <typename T, typename U>
    struct foo {
        T a;
        U b;
    };
} // namespace n219

auto main() -> int {
    {
        using namespace n219;

        foo f{ 10, 10.0 };
    }
    return 0;
}
