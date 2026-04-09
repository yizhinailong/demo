import std;

namespace n201 {

    template <typename T>
    auto add(T const a, T const b) -> T {
        return a + b;
    }

    class foo {
    public:
        explicit foo(int value)
            : m_value(value) {}

        explicit operator int() const {
            return m_value;
        }

    private:
        int m_value;
    };

    auto operator+(foo const a, foo const b) -> foo {
        return foo(int(a) + int(b));
    }

    template <typename Input, typename Predicate>
    auto count_if(Input start, Input end, Predicate p) {
        int total{ 0 };
        for (Input i = start; i != end; ++i) {
            if (p(*i)) {
                total++;
            }
        }
        return total;
    }

} // namespace n201

auto main() -> int {
    {
        using namespace n201;

        auto a1 = add(10, 20);
        auto a2 = add<int>(10, 20);
        auto a3 = add<>(10, 20);

        std::println("a1 = {}, a2 = {}, a3 = {}.", a1, a2, a3);

        auto b1 = add<short>(10, 20);
        // auto b2 = add(10.0, 20); // error
        auto b3 = add<double>(10.1, 20);

        std::println("b1 = {}, b3 = {}", b1, b3);

        auto f = add(foo(10), foo(20));

        std::println("f = {}", int(f));

        int arr[]{ 1, 1, 2, 3, 5, 8, 11 };
        int odds = count_if(
            std::begin(arr),
            std::end(arr),
            [](const int n) { return n % 2 == 1; }
        );
        std::println("odds = {}", odds);
    }

    return 0;
}
