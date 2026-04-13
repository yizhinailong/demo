import std;

namespace n312 {
    auto twice_as(int a, int b) -> bool {
        return a >= b * 2;
    }

    auto sum_and_div(double a, double b, double c) -> double {
        return (a + b) / c;
    }

    template <typename, typename>
    struct func_pair;

    template <typename R1, typename... A1, typename R2, typename... A2>
    struct func_pair<R1(A1...), R2(A2...)> {
        std::function<R1(A1...)> func1;
        std::function<R2(A2...)> func2;
    };
} // namespace n312

auto main() -> int {
    {
        using namespace n312;

        func_pair<bool(int, int), double(double, double, double)> func{ twice_as, sum_and_div };

        std::println("func.func1(1, 10) = {}", func.func1(1, 10));
        std::println("func.func2(1, 2, 3) = {}", func.func2(1, 2, 3));
    }
    return 0;
}
