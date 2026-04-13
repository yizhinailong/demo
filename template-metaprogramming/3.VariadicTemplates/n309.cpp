import std;

namespace n309 {
    template <typename T1, typename T2, typename T3, typename T4>
    constexpr auto get_type_sizes() -> std::array<std::size_t, 4> {
        return std::array<std::size_t, 4>{
            sizeof(T1),
            sizeof(T2),
            sizeof(T3),
            sizeof(T4)
        };
    }
} // namespace n309

auto main() -> int {
    {
        using namespace n309;

        auto sizes = get_type_sizes<int, float, double, long long>();
        for (auto const& s : sizes) {
            std::println("{}", s);
        }
    }
    return 0;
}
