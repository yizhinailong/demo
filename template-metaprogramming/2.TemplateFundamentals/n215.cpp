import std;

namespace n215 {
    template <auto... x>
    struct foo {};
} // namespace n215

auto main() -> int {
    {
        using namespace n215;

        [[maybe_unused]]
        foo<1, 10.0, false, '1'> f1;
    }
    return 0;
}
