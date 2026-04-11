import std;

namespace n220 {
    template <typename T, typename U = double>
    class foo {};

    // template <typename T = int, typename U> // error
    // class foo {};

} // namespace n220

auto main() -> int {
    return 0;
}
