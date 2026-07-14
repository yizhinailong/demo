import std;

namespace n425 {
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

    template <typename T>
    void h(T& v)
    {
        g(v);
    }

    template <typename T>
    void h(T&& v)
    {
        g(v);
    }
} // namespace n425

auto main() -> int {
    {
        using namespace n425;
        
        foo x{ 42 };
        
        h(x);          // g(foo&)
        h(foo{ 42 });  // g(foo&)
    }
    return 0;
}
