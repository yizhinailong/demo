import std;

namespace n635b {
    template <typename T>
        requires std::integral<T>
    using integral_vector = std::vector<T>;
} // namespace n635b

auto main() -> int {
    {
        using namespace n635b;
        
        integral_vector<int> v1{ 1,2,3 };
        //integral_vector<double> v2 {1.0, 2.0, 3.0}; // error
    }
    return 0;
}
