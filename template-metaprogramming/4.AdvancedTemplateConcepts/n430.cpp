import std;

namespace n430 {
    template <typename T>
    T minimum(T&& a, T&& b)
    {
        return a < b ? a : b;
    }
} // namespace n430

auto main() -> int {
    {
        using namespace n430;
        auto m1 = minimum(1, 5);         // OK
        auto m2 = minimum(18.49, 9.99);  // OK
        //auto m3 = minimum(1, 9.99);      // error, arguments of different type
    }
    return 0;
}
