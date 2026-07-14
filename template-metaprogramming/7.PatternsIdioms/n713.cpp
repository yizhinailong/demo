import std;

namespace n713 {
    class async_bool
    {
        std::function<bool()> check;
    public:
        async_bool() = delete;
        async_bool(std::function<bool()> checkIt)
            : check(checkIt)
        { }

        async_bool(bool val)
            : check([val]() {return val; })
        { }

        static async_bool yes() { return async_bool{ std::function<bool()>{ []() { return true; } } }; }
        static async_bool no() { return async_bool{ std::function<bool()>{ []() { return false; } } }; }

        bool operator&&(bool fore) const { return fore && check(); }
        bool operator!() const { return !check(); }
        operator bool() const { return check(); }
    };
} // namespace n713

auto main() -> int {
    {
        using namespace n713;
        
        async_bool b1{ false };
        async_bool b2{ true };
        async_bool b3{ std::function<bool()>{ []() {std::cout << "Y/N? "; char c; std::cin >> c; return c == 'Y' || c == 'y'; } } };
        
        if (b1) { std::cout << "b1 is true\n"; }
        if (b2) { std::cout << "b2 is true\n"; }
        if (b3) { std::cout << "b3 is true\n"; }
    }
    return 0;
}
