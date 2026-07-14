import std;

namespace n431 {
    template <typename T, typename U>
    auto minimum(T&& a, U&& b) -> decltype(a < b ? a : b)
    {
        return a < b ? a : b;
    }
} // namespace n431

auto main() -> int {
    {
        using namespace n431;
        auto m1 = minimum(1, 5);         // OK
        auto m2 = minimum(18.49, 9.99);  // OK
        auto m3 = minimum(1, 9.99);      // OK
    }
    return 0;
}
