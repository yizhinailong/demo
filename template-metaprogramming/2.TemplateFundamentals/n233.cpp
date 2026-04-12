import std;

namespace n233 {
    template <typename T>
    struct foo {
        static T value;
    };

    template <typename T>
    T foo<T>::value = 0;

    template <>
    int foo<int>::value = 10;
} // namespace n233

auto main() -> int {
    {
        using namespace n233;

        foo<double> a, b;
        std::println("a.value = {}", a.value);
        std::println("b.value = {}", b.value);

        foo<int> c;
        std::println("c.value = {}", c.value);

        a.value = 100.0;
        std::println("a.value = {}", a.value);
        std::println("b.value = {}", b.value);
        std::println("c.value = {}", c.value);
    }
    return 0;
}
