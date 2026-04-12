import std;

namespace n231 {
    template <typename T>
    struct foo {};

    template <typename T>
    auto func(foo<T>) -> void {
        std::println("This is primary template");
    };

    template <>
    auto func(foo<int>) -> void {
        std::println("This is int specialization template");
    }
} // namespace n231

auto main() -> int {
    {
        using namespace n231;

        func(foo<int>{});
        func(foo<double>{});
    }
    return 0;
}
