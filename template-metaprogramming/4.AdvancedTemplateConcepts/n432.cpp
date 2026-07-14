import std;

namespace n432 {
    template <typename T, typename U>
    decltype(auto) minimum(T&& a, U&& b)
    {
        return a < b ? a : b;
    }
} // namespace n432

auto main() -> int {
    {
        using namespace n432;
        auto m1 = minimum(1, 5);         // OK
        auto m2 = minimum(18.49, 9.99);  // OK
        auto m3 = minimum(1, 9.99);      // OK
    }
    return 0;
}
