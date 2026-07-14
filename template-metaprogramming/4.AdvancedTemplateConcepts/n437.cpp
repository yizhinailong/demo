import std;

namespace n436 {
    template <typename T, typename U>
    struct composition
    {
        using result_type = decltype(T{} + U{});
    };

    struct wrapper
    {
        wrapper(int const v) : value(v){}

        int value;

        friend wrapper operator+(int const a, wrapper const& w)
        {
            return wrapper(a + w.value);
        }

        friend wrapper operator+(wrapper const& w, int const a)
        {
            return wrapper(a + w.value);
        }
    };
} // namespace n436

namespace n437 {
    using n436::wrapper;

    template <typename T, typename U>
    struct composition
    {
        using result_type = decltype(std::declval<T>() + std::declval<U>());
    };
} // namespace n437

auto main() -> int {
    {
        using namespace n437;
        
        static_assert(
           std::is_same_v<
           double,
           composition<int, double>::result_type>);
        
        static_assert(
           std::is_same_v<
           wrapper,
           composition<int, wrapper>::result_type>);
    }
    return 0;
}
