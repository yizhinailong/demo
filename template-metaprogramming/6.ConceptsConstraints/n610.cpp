import std;

namespace n610 {
    template <typename T, typename U = void>
    struct is_container : std::false_type {};

    template <typename T>
    struct is_container<T,
        std::void_t<typename T::value_type,
                        typename T::size_type,
                        typename T::allocator_type,
                        typename T::iterator,
                        typename T::const_iterator,
                        decltype(std::declval<T>().size()),
                        decltype(std::declval<T>().begin()),
                        decltype(std::declval<T>().end()),
                        decltype(std::declval<T>().cbegin()),
                        decltype(std::declval<T>().cend())>> 
        : std::true_type{};

    template <typename T, typename U = void>
    constexpr bool is_container_v = is_container<T, U>::value;

    struct foo {};

    static_assert(!is_container_v<foo>);
    static_assert(is_container_v<std::vector<foo>>);
} // namespace n610

auto main() -> int {
    return 0;
}
