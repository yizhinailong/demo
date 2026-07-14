import std;

namespace n424 {
    template <typename T>
    void f(T&& arg)               // forwarding reference
    { std::cout << "f(T&&)\n"; }

    template <typename T>
    void f(T const&& arg)         // rvalue reference
    { std::cout << "f(T const&&)\n"; }

    template <typename T>
    void f(std::vector<T>&& arg)  // rvalue reference
    { std::cout << "f(vector<T>&&)\n"; }

    template <typename T>
    struct S
    {
        void f(T&& arg)            // rvalue reference
        { std::cout << "S.f(T&&)\n"; }
    };
} // namespace n424

auto main() -> int {
    {
        using namespace n424;
        
        int x = 42;
        f(x);                   // f(T&&)
        f(42);                  // f(T&&)
        
        int const cx = 100;
        f(cx);                  // f(T&&)
        f(std::move(cx));       // f(T const&&)
        
        std::vector<int> v{ 42 };
        f(v);                   // f(T&&)
        f(std::vector<int>{42});// f(vector<T>&&)
        
        S<int> s;
        //s.f(x);                 // error
        s.f(42);                // S.f(T&&)
    }

    {
        using lrefint = int&;
        using rrefint = int&&;
        int x = 42;
        lrefint& r1 = x; // type of r1 is int&
        lrefint&& r2 = x; // type of r2 is int&
        rrefint& r3 = x; // type of r3 is int&
        rrefint&& r4 = 1; // type of r4 is int&&
    }

    {
        int x = 42;
        auto&& rx = x;          // int&
        auto&& rc = 42;         // int&&
        //auto const&& rcx = x;   // error
        
        std::vector<int> v{ 42 };
        auto&& rv = v[0];       // int&
    }
    return 0;
}
