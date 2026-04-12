import std;

namespace n241 {
    template <typename T>
    constexpr T PI = T(3.1415926535897932385L);

    template <typename T>
    auto sphere_volume(T const r) -> T {
        return 4 * PI<T> * r * r * r / 3;
    }
} // namespace n241

auto main() -> int {
    {
        using namespace n241;

        float  v1 = sphere_volume(42.0f);
        double v2 = sphere_volume(42.0);

        std::println("v1 = {}", v1);
        std::println("v2 = {}", v2);
    }
    return 0;
}
