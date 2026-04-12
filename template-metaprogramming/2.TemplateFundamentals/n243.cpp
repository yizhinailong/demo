import std;

namespace n243 {
    struct math_constants {
        template <class T>
        static const T PI;
    };

    template <class T>
    const T math_constants::PI = T(3.1415926535897932385L);

    template <typename T>
    T sphere_volume(T const r) {
        return 4 * math_constants::PI<T> * r * r * r / 3;
    }
} // namespace n243

auto main() -> int {
    {
        using namespace n243;

        float  v1 = sphere_volume(42.0f);
        double v2 = sphere_volume(42.0);

        std::println("v1 = {}", v1);
        std::println("v2 = {}", v2);
    }
    return 0;
}
