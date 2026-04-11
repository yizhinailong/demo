import std;

namespace n214 {

    template <std::size_t N>
    struct string_literal {
        constexpr string_literal(char const (&str)[N]) {
            std::copy_n(str, N, m_value);
        }

        char m_value[N];
    };

    template <string_literal x>
    struct foo {};

} // namespace n214

auto main() -> int {
    {
        using namespace n214;

        [[maybe_unused]]
        foo<"42"> f1;
    }
    return 0;
}
