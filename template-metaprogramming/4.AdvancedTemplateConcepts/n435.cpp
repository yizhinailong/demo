import std;

namespace n435 {
    template <typename T>
    T const& func(T const& ref)
    {
        return ref;
    }

    template <typename T>
    decltype(auto) func_caller(T&& ref)
    {
        return func(std::forward<T>(ref));
    }
} // namespace n435

auto main() -> int {
    {
        using namespace n435;
        
        int a = 42;
        
        decltype(func(a)) r1 = func(a);                 // int const&
        decltype(func_caller(a)) r2 = func_caller(a);   // int const&
    }
    return 0;
}
