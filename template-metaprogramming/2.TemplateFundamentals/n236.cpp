import std;

namespace n236 {
    template <int A, int B>
    struct foo {};

    template <int A>
    struct foo<A, A> {};

    // template <int A>
    // struct foo<A, A + 1> {};
} // namespace n236

auto main() -> int {
    return 0;
}
