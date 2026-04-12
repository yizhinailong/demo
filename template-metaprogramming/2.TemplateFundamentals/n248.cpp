import std;

auto main() -> int {
    {
        int arr[] = { 1, 6, 3, 8, 4, 2, 9 };
        std::sort(
            std::begin(arr),
            std::end(arr),
            [](int const a, int const b) { return a > b; }
        );

        int  pivot = 5;
        auto count = std::count_if(
            std::begin(arr),
            std::end(arr),
            [pivot](int const a) { return a > pivot; }
        );

        std::cout << count << '\n';
    }

    {
        auto l1 = [](int a) {
            return a + a;
        }; // C++11, regular lambda
        auto l2 = [](auto a) {
            return a + a;
        }; // C++14, generic lambda
        auto l3 = []<typename T>(T a) {
            return a + a;
        }; // C++20, template lambda

        auto v1  = l1(42);   // OK
        auto v2  = l1(42.0); // warning
        // auto v3 = l1(std::string{ "42" });   // error

        auto v5  = l2(42);                  // OK
        auto v6  = l2(42.0);                // OK
        auto v7  = l2(std::string{ "42" }); // OK

        auto v8  = l3(42);                  // OK
        auto v9  = l3(42.0);                // OK
        auto v10 = l3(std::string{ "42" }); // OK
    }

    {
        auto l1 = [](int a, int b) {
            return a + b;
        };
        auto l2 = [](auto a, auto b) {
            return a + b;
        };
        auto l3 = []<typename T, typename U>(T a, U b) {
            return a + b;
        };
        auto l4 = [](auto a, decltype(a) b) {
            return a + b;
        };

        auto v1  = l1(42, 1);     // OK
        auto v2  = l1(42.0, 1.0); // warning

        // auto v3 = l1(std::string{ "42" }, '1'); // error

        auto v4  = l2(42, 1);                                    // OK
        auto v5  = l2(42.0, 1);                                  // OK
        auto v6  = l2(std::string{ "42" }, '1');                 // OK
        auto v7  = l2(std::string{ "42" }, std::string{ "1" });  // OK

        auto v8  = l3(42, 1);                                    // OK
        auto v9  = l3(42.0, 1);                                  // OK
        auto v10 = l3(std::string{ "42" }, '1');                 // OK
        auto v11 = l3(std::string{ "42" }, std::string{ "42" }); // OK

        auto v12 = l4(42.0, 1);                                  // OK
        auto v13 = l4(42, 1.0);                                  // warning

        // auto v14 = l4(std::string{ "42" }, '1');                 // error
    }

    {
        auto l = []<typename T, std::size_t N>(std::array<T, N> const& arr) {
            return std::accumulate(arr.begin(), arr.end(), static_cast<T>(0));
        };

        // auto v1 = l(1); // error
        auto v2 = l(std::array<int, 3>{ 1, 2, 3 });
    }

    {
        auto l = []<typename T>(T a, T b) {
            return a + b;
        };

        auto v1 = l(42, 1); // OK
        // auto v2 = l(42.0, 1);    // error

        auto v4 = l(42.0, 1.0); // OK
        // auto v5 = l(42, false);    // error

        auto v6 = l(std::string{ "42" }, std::string{ "1" }); // OK

        // auto v6 = l(std::string{ "42" }, '1');                // error
    }

    {
        std::function<int(int)> factorial;
        factorial = [&factorial](int const n) {
            if (n < 2) {
                return 1;
            } else {
                return n * factorial(n - 1);
            }
        };

        std::cout << factorial(5) << '\n';
    }

    {
        auto factorial = [](auto f, int const n) {
            if (n < 2) {
                return 1;
            } else {
                return n * f(f, n - 1);
            }
        };

        std::cout << factorial(factorial, 5) << '\n';
    }
    return 0;
}
