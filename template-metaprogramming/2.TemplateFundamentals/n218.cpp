import std;

namespace n218 {
    template <typename T, typename U = int>
    class foo {};

    // template <typename T = int, typename U> // error
    // class bar {};
} // namespace n218

auto main() -> int {
    return 0;
}
