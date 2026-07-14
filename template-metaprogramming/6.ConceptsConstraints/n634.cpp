import std;

namespace n634 {
    template <std::floating_point T>
    constexpr T PI = T(3.1415926535897932385L);
} // namespace n634

auto main() -> int {
    {
        using namespace n634;
        
        std::cout << PI<double> << '\n';  // OK
        //std::cout << PI<int> << '\n';     // error
    }
    return 0;
}
