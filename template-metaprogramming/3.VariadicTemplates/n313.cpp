import std;

namespace n313 {
    template <typename T, typename... Ts>
    struct tuple {
        tuple(T t, Ts... ts)
            : value(t), rest(ts...) {}

        constexpr auto size() const -> std::size_t {
            return 1 + rest.size();
        }

        T            value;
        tuple<Ts...> rest;
    };

    template <typename T>
    struct tuple<T> {
        tuple(T t)
            : value(t) {}

        constexpr auto size() const -> std::size_t {
            return 1;
        }

        T value;
    };

    template <std::size_t N, typename T, typename... Ts>
    struct nth_type : nth_type<N - 1, Ts...> {
        static_assert(N < sizeof...(Ts) + 1, "Index out of bounds.");
    };

    template <typename T, typename... Ts>
    struct nth_type<0, T, Ts...> {
        using value_type = T;
    };

    template <size_t N>
    struct getter {
        template <typename... Ts>
        static auto get(tuple<Ts...>& t) -> typename nth_type<N, Ts...>::value_type& {
            return getter<N - 1>::get(t.rest);
        }
    };

    template <>
    struct getter<0> {
        template <typename T, typename... Ts>
        static auto get(tuple<T, Ts...>& t) -> T& {
            return t.value;
        }
    };

    template <size_t N, typename... Ts>
    auto get(tuple<Ts...>& t) -> typename nth_type<N, Ts...>::value_type& {
        return getter<N>::get(t);
    }
} // namespace n313

auto main() -> int {
    {
        using namespace n313;

        tuple<int>               one(42);
        tuple<int, double>       two(42, 42.0);
        tuple<int, double, char> three(42, 42.0, 'a');

        std::println("{}", one.value);
        std::println("{},{}", two.value, two.rest.value);
        std::println("{},{},{}", three.value, three.rest.value, three.rest.rest.value);

        std::println("{}", get<0>(one));
        std::println("{},{}", get<0>(two), get<1>(two));
        std::println("{},{},{}", get<0>(three), get<1>(three), get<2>(three));
    }
    return 0;
}
