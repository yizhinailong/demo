import std;

namespace n616 {
    template<typename T, typename... Ts>
    inline constexpr bool are_same_v = std::conjunction_v<std::is_same<T, Ts>...>;

    template <typename ... T>
    concept HomogenousRange = requires(T... t)
    {
        (... + t);
        requires are_same_v<T...>;
        requires sizeof...(T) > 1;
    };

    template <typename ... T>
    requires HomogenousRange<T...>
    auto add(T&&... t)
    {
        return (... + t);
    }

    static_assert(HomogenousRange<int, int>);
    static_assert(!HomogenousRange<int>);
    static_assert(!HomogenousRange<int, double>);
} // namespace n616

auto main() -> int {
    {
        using namespace n616;
        
        add(1, 2);
        //add(1);
        //add(1, 2.0);
    }
    return 0;
}
