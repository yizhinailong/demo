import std;

namespace n242 {
    struct math_constants {
        template <class T>
        static constexpr T PI = T(3.1415926535897932385L);
    };

    template <typename T>
    auto sphere_volume(T const r) -> T {
        return 4 * math_constants::PI<T> * r * r * r / 3;
    }
} // namespace n242

auto main() -> int {
    {
        using namespace n242;

        float  v1 = sphere_volume(42.0f);
        double v2 = sphere_volume(42.0);

        std::println("v1 = {}", v1);
        std::println("v2 = {}", v2);
    }
    return 0;
}
