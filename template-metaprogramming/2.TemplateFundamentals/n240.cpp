import std;

namespace n240 {
    template <typename T>
    struct PI {
        static const T value;
    };

    template <typename T>
    const T PI<T>::value = T(3.1415926535897932385L);

    template <typename T>
    T sphere_volume(T const r) {
        return 4 * PI<T>::value * r * r * r / 3;
    }
} // namespace n240

auto main() -> int {
    {
        using namespace n240;

        float  v1 = sphere_volume(10.0f);
        double v2 = sphere_volume(10.0);

        std::println("v1 = {}", v1);
        std::println("v2 = {}", v2);
    }
    return 0;
}
