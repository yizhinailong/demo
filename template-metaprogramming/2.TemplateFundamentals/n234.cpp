import std;

namespace n234 {
    template <typename T, typename U>
    auto func(T a, U b) -> void {
        std::println("This is primary template.");
    }

    template <>
    auto func(int a, int b) -> void {
        std::println("This is <int, int> specilization template.");
    }

    template <>
    auto func(int a, double b) -> void {
        std::println("This is <int, double> specilization template.");
    }
} // namespace n234

auto main() -> int {
    {
        using namespace n234;

        func(10, 10);
        func(10, 10.0);
        func(10.0, 10.0);
    }
    return 0;
}
