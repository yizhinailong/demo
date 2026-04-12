import std;

namespace n227 {
    template <typename T>
    class foo {
        struct bar {};

        auto f(bar const arg) -> T {
            return {};
        }
    };

    template int foo<int>::f(foo<int>::bar);
} // namespace n227

auto main() -> int {
    return 0;
}
