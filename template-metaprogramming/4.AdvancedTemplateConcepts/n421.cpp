import std;

namespace n421 {
    struct foo
    {
        int data;
    };

    void f(foo& v)
    {
        std::cout << "f(foo&)\n";
    }

    void g(foo& v)
    {
        std::cout << "g(foo&)\n";
    }

    void g(foo&& v)
    {
        std::cout << "g(foo&&)\n";
    }

    void h(foo&& v)
    {
        std::cout << "h(foo&&)\n";
    }
} // namespace n421

auto main() -> int {
    {
        using namespace n421;
        
        foo x = { 42 };   // x is l-value
        foo& rx = x;      // rx is l-value
        
        f(x);             // f(foo&)
        f(rx);            // f(foo&)
        //f(foo{ 42 });     // error, a non-const reference may only be bound to an lvalue
    }

    {
        using namespace n421;
        
        foo x = { 42 };   // x is l-value
        foo& rx = x;      // rx is l-value
        
        g(x);             // g(foo&)
        g(rx);            // g(foo&)
        g(foo{ 42 });     // g(foo&&)
    }

    {
        using namespace n421;
        
        foo x = { 42 };   // x is l-value
        foo& rx = x;      // rx is l-value
        
        //h(x);             // error, cannot bind an lvalue to an rvalue reference
        //h(rx);         // error
        h(foo{ 42 });  // h(foo&&)
        h(std::move(x));  // h(foo&7)
    }
    return 0;
}
