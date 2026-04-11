import std;

namespace n222 {
    template <typename T>
    struct foo {
        void f() {
        }

        void g() {
            int a = 42;
        }
    };
} // namespace n222

auto main() -> int {
    {
        using namespace n222;

        foo<int> f;
        f.f();
        f.g();
    }
    return 0;
}
