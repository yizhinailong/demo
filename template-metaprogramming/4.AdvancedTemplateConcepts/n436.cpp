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

auto main() -> int {
    {
        using namespace n436;
        
        static_assert(
           std::is_same_v<
           double,
           composition<int, double>::result_type>);
        
        // error, no appropriate default constructor available
        /*
        static_assert(
           std::is_same_v<
           wrapper,
           composition<int, wrapper>::result_type>);
           */
    }
    return 0;
}
