import std;

namespace n415 {
    template <typename T>
    void invoke(void (*pfun)(T, int)) {
        pfun(T{}, 42);
    }

    template <typename T>
    void alpha(T, int) {
        std::cout << "alpha(T,int)" << '\n';
    }

    void beta(int, int) {
        std::cout << "beta(int,int)" << '\n';
    }

    void beta(short, int) {
        std::cout << "beta(short,int)" << '\n';
    }

    void gamma(short, int, long long) {
        std::cout << "gamma(short,int,long long)" << '\n';
    }

    void gamma(double, int) {
        std::cout << "gamma(double,int)" << '\n';
    }
} // namespace n415

auto main() -> int {
    {
        using namespace n415;

        // invoke(&alpha);
        // invoke(&beta);
        invoke(&gamma);
    }
    return 0;
}
