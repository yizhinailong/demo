import std;

namespace n511 {
    template <typename T>
    bool are_equal(T const& a, T const& b)
    {
        if constexpr (std::is_floating_point_v<T>)
            return std::abs(a - b) < 0.001;
        else
            return a == b;
    }
} // namespace n511

auto main() -> int {
    {
        using namespace n511;
        
        are_equal(1, 1);
        are_equal(1.999998, 1.999997);
        are_equal(std::string{ "1" }, std::string{ "1" });
        //are_equal(n502::widget{ 1, "one" }, n502::widget{ 1, "two" });
    }
    return 0;
}
