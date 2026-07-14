import std;

namespace n428 {
    int f() { return 42; }

    int g() { return 0; }
    int g(int a) { return a; }
    int g(int a, int b) { return a + b; }

    struct wrapper
    {
        int val;

        int get() const { return val; }
    };
} // namespace n428

auto main() -> int {
    {
        using namespace n428;
        
        int a = 42;
        int& ra = a;
        const double d = 42.99;
        long arr[10];
        long l = 0;
        char* p = nullptr;
        char c = 'x';
        wrapper w1{ 1 };
        wrapper* w2 = new wrapper{ 2 };
        
        [[maybe_unused]] decltype(a) e1;             // R1, int
        [[maybe_unused]] decltype(ra) e2 = a;        // R1, int&
        [[maybe_unused]] decltype(f) e3;             // R1, int()
        [[maybe_unused]] decltype(f()) e4;           // R2, int
        //[[maybe_unused]] decltype(g) e5;           // R1, error
        [[maybe_unused]] decltype(g(1)) e6;          // R2, int
        [[maybe_unused]] decltype(&f) e7 = nullptr;  // R4, int(*)()
        [[maybe_unused]] decltype(d) e8 = 1;         // R1, const double
        [[maybe_unused]] decltype(arr) e9;           // R1, long[10]
        [[maybe_unused]] decltype(arr[1]) e10 = l;   // R3, long&
        [[maybe_unused]] decltype(w1.val) e11;       // R1, int
        [[maybe_unused]] decltype(w1.get()) e12;     // R1, int
        [[maybe_unused]] decltype(w2->val) e13;      // R1, int
        [[maybe_unused]] decltype(w2->get()) e14;    // R1, int
        [[maybe_unused]] decltype(42) e15 = 1;       // R4, int
        [[maybe_unused]] decltype(1 + 2) e16;        // R4, int
        [[maybe_unused]] decltype(a + 1) e17;        // R4, int
        [[maybe_unused]] decltype(a = 0) e18 = a;    // R3, int&
        [[maybe_unused]] decltype(p) e19 = nullptr;  // R1, char*
        [[maybe_unused]] decltype(*p) e20 = c;       // R3, char&
        [[maybe_unused]] decltype(p[0]) e21 = c;     // R3, char&
        
        delete w2;
    }

    {
        int a = 42;
        decltype(a = 1) e = a;
        std::cout << a << '\n';
    }
    return 0;
}
