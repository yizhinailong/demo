import std;

namespace n232 {
    template <typename T>
    auto func(T a) -> void {
        std::print("This is primary template.");
    }

    template <>
    auto func(int a) -> void {
        std::println("This is int specilization template.");
    }
} // namespace n232

auto main() -> int {
    {
        using namespace n232;

        func(10);
        func(10.0);
    }
    return 0;
}
