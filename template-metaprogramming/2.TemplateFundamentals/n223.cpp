import std;

namespace n223 {
    template <typename T>
    struct foo {
        void f() {}

        void g() {}
    };
} // namespace n223

auto main() -> int {
    {
        using namespace n223;

        [[maybe_unused]] foo<int>    a1;
        [[maybe_unused]] foo<int>*   a2;
        [[maybe_unused]] foo<double> a3;
    }
    {
        using namespace n223;

        [[maybe_unused]]
        foo<int>*    p;
        foo<int>     x;
        foo<double>* q = nullptr;

        x.f();
        q->g();
    }
    return 0;
}
