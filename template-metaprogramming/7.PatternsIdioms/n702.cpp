import std;

namespace n702 {
    template <typename T>
    struct Base
    {
        void f() { static_cast<T*>(this)->f(); }
    };

    struct Derived : public Base<Derived>
    {
        void f() { std::cout << "Derived::f()\n"; }
    };

    template <typename T>
    void process(Base<T>& b)
    {
        b.f();
    }
} // namespace n702

auto main() -> int {
    {
        using namespace n702;
        
        Derived d;
        process(d);
    }
    return 0;
}
