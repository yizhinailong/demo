import std;

namespace n244 {
    template <typename T>
    constexpr T SEPARATOR = '\n';

    template <>
    constexpr wchar_t SEPARATOR<wchar_t> = L'\n';

    template <typename T>
    auto show_parts(
        std::basic_ostream<T>&           s,
        std::basic_string_view<T> const& str
    ) -> std::basic_ostream<T>& {
        using size_type = typename std::basic_string_view<T>::size_type;
        size_type start = 0;
        size_type end;
        do {
            end = str.find(SEPARATOR<T>, start);
            s << '[' << str.substr(start, end - start) << ']' << SEPARATOR<T>;
            start = end + 1;
        } while (end != std::string::npos);
        return s;
    }
} // namespace n244

auto main() -> int {
    {
        using namespace n244;

        show_parts<char>(std::cout, "one\ntwo\nthree");
        show_parts<wchar_t>(std::wcout, L"one line");
    }
    return 0;
}
