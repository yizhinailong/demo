import std;

namespace n230 {
    template <typename>
    struct foo {}; // primary template

    template <>
    struct foo<int>; // explicit specialization declaration
} // namespace n230

auto main() -> int {
    {
        using namespace n230;

        [[maybe_unused]] foo<double> a; // ok
        [[maybe_unused]] foo<int>*   b; // ok
        // foo<int> c; // error
    }
    return 0;
}
