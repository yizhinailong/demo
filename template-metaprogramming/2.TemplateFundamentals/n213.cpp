import std;

namespace n213 {
    template <auto x>
    struct foo {};
} // namespace n213

auto main() -> int {
    {
        using namespace n213;

        [[maybe_unused]]
        foo<42> f1;

        [[maybe_unused]]
        foo<40.0> f2;

        // foo<"1"> f2; // error
    }
    return 0;
}
