import std;

namespace n237 {
    template <typename T, std::size_t S>
    auto pretty_print(std::ostream& os, std::array<T, S> const& arr) -> std::ostream& {
        os << '[';
        if (S > 0) {
            for (std::size_t i = 0; i < S - 1; ++i) {
                os << arr[i] << ", ";
            }
            os << arr[S - 1];
        }
        os << "]\n";
        return os;
    }
} // namespace n237

auto main() -> int {
    {
        using namespace n237;

        std::array<int, 8> arr{ 1, 2, 3, 4, 5, 6, 7, 8 };
        pretty_print(std::cout, arr);

        std::array<char, 8>        arr_char{};
        constexpr std::string_view sv = "template";
        std::ranges::copy(sv, arr_char.begin());
        pretty_print(std::cout, arr_char);
    }
    return 0;
}
