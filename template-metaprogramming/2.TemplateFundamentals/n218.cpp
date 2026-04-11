import std;

namespace n219 {
    template <typename T, typename U = int>
    class foo {};

    // template <typename T = int, typename U> // error
    // class bar {};
} // namespace n219

auto main() -> int {
    return 0;
}
