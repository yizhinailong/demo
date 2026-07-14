import std;

namespace n519 {
    template <typename, typename... Ts>
    struct has_common_type : std::false_type {};

    template <typename... Ts>
    struct has_common_type<std::void_t<std::common_type_t<Ts...>>, Ts...>
        : std::true_type {};

    template <typename... Ts>
    constexpr bool has_common_type_v =
        sizeof...(Ts) < 2 ||
        has_common_type<void, Ts...>::value;

    template<typename... Ts,
                typename = std::enable_if_t<has_common_type_v<Ts...>>>
    void process(Ts&&... ts) 
    {
        //static_assert(has_common_type_v<Ts...>, 
//                    "Arguments must have a common type.");

        std::cout << typeid(std::common_type_t<Ts...>).name() << '\n';
    }
} // namespace n519

auto main() -> int {
    {
        using namespace n519;
        
        process(1);              // int
        process(1, 2, 3);        // int
        process(1, 2.0, '3');    // double
        //process(1, 2.0, "3");    // error
    }
    return 0;
}
