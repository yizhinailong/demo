import std;

namespace n239 {
    constexpr double PI = 3.1415926535897932385L;

    template <typename T>
    auto sphere_volume(T const r) -> T {
        return static_cast<T>(4 * PI * r * r * r / 3);
    }
} // namespace n239

auto main() -> int {
    {
        using namespace n239;

        float  v1 = sphere_volume(10.0f);
        double v2 = sphere_volume(10.0);

        std::println("v1 = {}", v1);
        std::println("v2 = {}", v2);
    }
    return 0;
}
