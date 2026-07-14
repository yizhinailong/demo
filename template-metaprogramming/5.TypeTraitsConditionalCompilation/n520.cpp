import std;

namespace n520 {
    namespace detail
    {
        template <bool b>
        struct copy_fn
        {
            template<typename InputIt, typename OutputIt>
            constexpr static OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
            {
                while (first != last)
                {
                    *d_first++ = *first++;
                }
                return d_first;
            }
        };

        template <>
        struct copy_fn<true>
        {
            template<typename InputIt, typename OutputIt>
            constexpr static OutputIt* copy(InputIt* first, InputIt* last, OutputIt* d_first)
            {
                std::memmove(d_first, first, (last - first) * sizeof(InputIt));
                return d_first + (last - first);
            }
        };
    }

    template<typename InputIt, typename OutputIt>
    constexpr OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
    {
        using input_type = std::remove_cv_t<typename std::iterator_traits<InputIt>::value_type>;
        using output_type = std::remove_cv_t<typename std::iterator_traits<OutputIt>::value_type>;

        constexpr bool opt =
            std::is_same_v<input_type, output_type> &&
            std::is_pointer_v<InputIt> &&
            std::is_pointer_v<OutputIt> &&
            std::is_trivially_copy_assignable_v<input_type>;

        return detail::copy_fn<opt>::copy(first, last, d_first);
    }
} // namespace n520

auto main() -> int {
    {
        std::vector<int> v1{ 1, 2, 3, 4, 5 };
        std::vector<int> v2(5);
        
        n520::copy(std::begin(v1), std::end(v1), std::begin(v2));
        
        int a1[5] = { 1,2,3,4,5 };
        int a2[5];
        
        n520::copy(a1, a1 + 5, a2);
    }
    return 0;
}
