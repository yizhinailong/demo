import std;

namespace n317 {
    template <typename T, typename... Args>
    struct foo {
    };

    template <typename... Args>
    using int_foo = foo<int, Args...>;

    template <typename T, T... Ints>
    struct integer_sequence {
    };

    template <std::size_t... Ints>
    using index_sequence = integer_sequence<std::size_t, Ints...>;

    template <typename T, std::size_t N, T... Is>
    struct make_integer_sequence : make_integer_sequence<T, N - 1, N - 1, Is...> {};

    template <typename T, T... Is>
    struct make_integer_sequence<T, 0, Is...> : integer_sequence<T, Is...> {};

    template <std::size_t N>
    using make_index_sequence = make_integer_sequence<std::size_t, N>;

    template <typename... T>
    using index_sequence_for = make_index_sequence<sizeof...(T)>;

    template <typename Tuple, std::size_t... Ints>
    auto select_tuple(Tuple&& tuple, index_sequence<Ints...>) {
        return std::make_tuple(
            std::get<Ints>(std::forward<Tuple>(tuple))...
        );
    }
} // namespace n317

auto main() -> int {
    {
        using namespace n317;

        [[maybe_unused]]
        foo<double, char, int> f1;
        foo<int, char, double> f2;
        int_foo<char, double>  f3;
        static_assert(std::is_same_v<decltype(f2), decltype(f3)>);
    }

    {
        using namespace n317;

        std::tuple<int, char, double> t1{ 42, 'x', 42.99 };
        auto                          t2 = select_tuple(t1, index_sequence<0, 2>{});
    }

    return 0;
}
