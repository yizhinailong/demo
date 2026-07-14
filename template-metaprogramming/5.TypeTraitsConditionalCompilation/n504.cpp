import std;

namespace n504 {
    template <typename T>
    struct foo
    {
        using foo_type = T;
    };

    template <typename T>
    struct bar
    {
        using bar_type = T;
    };

    struct int_foo : foo<int> {};
    struct int_bar : bar<int> {};

    template <typename T>
    decltype(typename T::foo_type(), void()) handle(T const& v)
    {
        std::cout << "handle a foo\n";
    }

    template <typename T>
    decltype(typename T::bar_type(), void()) handle(T const& v)
    {
        std::cout << "handle a bar\n";
    }
} // namespace n504

auto main() -> int {
    {
        using namespace n504;
        
        int_foo fi;
        int_bar bi;
        int x = 0;
        handle(fi);
        handle(bi);
        //handle(x);
    }
    return 0;
}
