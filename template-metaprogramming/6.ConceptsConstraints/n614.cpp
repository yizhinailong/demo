import std;

namespace n614 {
    template <typename T>
    void f(T) noexcept {}

    template <typename T>
    void g(T) {}

    template <typename F, typename ... T>
    concept NonThrowing = requires(F && func, T ... t)
    {
        {func(t...)} noexcept;
    };

    template <typename F, typename ... T>
        requires NonThrowing<F, T...>
    void invoke(F&& func, T... t)
    {
        func(t...);
    }
} // namespace n614

auto main() -> int {
    {
        using namespace n614;
        
        invoke(f<int>, 42);
        //invoke(g<int>, 42); // error
    }
    return 0;
}
