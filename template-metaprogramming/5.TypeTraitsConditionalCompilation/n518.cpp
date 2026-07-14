import std;

namespace n518 {
    template <typename T, size_t S>
    using list_t = typename std::conditional<S == 1, T, std::vector<T>>::type;
} // namespace n518

auto main() -> int {
    {
        using namespace n518;
        
        static_assert(std::is_same_v<list_t<int, 1>, int>);
        static_assert(std::is_same_v<list_t<int, 2>, std::vector<int>>);
    }
    return 0;
}
