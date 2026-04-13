import std;

namespace n302 {
#include <stdarg.h>

    template <typename T>
    T min(int count, ...) {
        va_list args;
        va_start(args, count);

        T val = va_arg(args, T);
        for (int i = 1; i < count; i++) {
            T n = va_arg(args, T);
            if (n < val) {
                val = n;
            }
        }

        va_end(args);

        return val;
    }
} // namespace n302

auto main() -> int {
    {
        using namespace n302;

        std::println("min<int>(2, 42, 7) = {}", min<int>(2, 42, 7));
        std::println("min<double>(5, 1, 5, 3, -4, 9.0) = {}", min<double>(5, 1, 5, 3, -4, 9.0));
    }
    return 0;
}
