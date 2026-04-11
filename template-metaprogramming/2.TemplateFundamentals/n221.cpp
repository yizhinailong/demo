import std;

namespace n221 {
    template <typename T>
    class foo {
    public:
        using value_type = T;
    };

    template <typename T, typename U = T::value_type>
    struct bar {
        using value_type = U;
    };
} // namespace n221

auto main() -> int {
    {
        using namespace n221;

        std::println(
            "std::is_same_v<int, bar<foo<int>>::value_type> = {}",
            std::is_same_v<int, bar<foo<int>>::value_type>
        );
    }
    return 0;
}
