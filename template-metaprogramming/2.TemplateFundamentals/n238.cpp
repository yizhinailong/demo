import std;

namespace n238 {
    template <std::size_t S>
    auto pretty_print(std::ostream& os, std::array<char, S> const arrs) -> std::ostream& {
        os << '[';
        for (auto& arr : arrs) {
            os << arr;
        }
        os << ']';
        return os;
    }
} // namespace n238

auto main() -> int {
    {
        using namespace n238;

        std::array<char, 8>        arr_char{};
        constexpr std::string_view sv = "template";
        std::ranges::copy(sv, arr_char.begin());
        pretty_print(std::cout, arr_char);
    }
    return 0;
}
