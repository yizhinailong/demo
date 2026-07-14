import std;

namespace n635a {
    template <std::integral T>
    using integral_vector = std::vector<T>;
} // namespace n635a

auto main() -> int {
    {
        using namespace n635a;
        
        integral_vector<int> v1 { 1,2,3 };
        //integral_vector<double> v2 {1.0, 2.0, 3.0}; // error
    }
    return 0;
}
