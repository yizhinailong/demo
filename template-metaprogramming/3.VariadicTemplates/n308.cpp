import std;

namespace n308 {
    template <typename... Ts>
    constexpr auto get_type_sizes() -> std::array<std::size_t, sizeof...(Ts)> {
        return std::array<std::size_t, sizeof...(Ts)>{ sizeof(Ts)... };
    }
} // namespace n308

auto main() -> int {
    {
        using namespace n308;

        auto sizes = get_type_sizes<int, float, double, long long>();
        for (auto const& s : sizes) {
            std::println("{}", s);
        }
    }
    return 0;
}
