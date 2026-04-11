import std;

namespace n225 {
    template <typename T>
    struct foo {
        static T data;
    };

    template <typename T>
    T foo<T>::data = 0;
} // namespace n225

auto main() -> int {
    {
        using namespace n225;

        foo<int>    a;
        foo<double> b;
        foo<double> c;

        std::println("foo<int> a = {}", a.data);
        std::println("foo<double> b = {}", b.data);
        std::println("foo<double> c = {}", c.data);

        b.data = 42;
        std::println("foo<double> b = {}", b.data);
        std::println("foo<double> c = {}", c.data);
    }
    return 0;
}
