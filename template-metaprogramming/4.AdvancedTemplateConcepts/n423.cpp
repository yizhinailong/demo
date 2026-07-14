import std;

namespace n423 {
    struct foo
    {
        int data;
    };

    void g(foo& v)
    {
        std::cout << "g(foo&)\n";
    }

    void g(foo&& v)
    {
        std::cout << "g(foo&&)\n";
    }

    void h(foo& v)
    {
        g(std::forward<foo&>(v));
    }

    void h(foo&& v)
    {
        g(std::forward<foo&&>(v));
    }
} // namespace n423

auto main() -> int {
    {
        using namespace n423;
        
        foo x{ 42 };
        
        h(x);          // g(foo&)
        h(foo{ 42 });  // g(foo&&)
    }
    return 0;
}
