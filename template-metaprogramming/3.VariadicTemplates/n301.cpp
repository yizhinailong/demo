import std;

namespace n301 {
#include <stdarg.h>

    int min(int count, ...) {
        va_list args;
        va_start(args, count);

        int val = va_arg(args, int);
        for (int i = 1; i < count; i++) {
            int n = va_arg(args, int);
            if (n < val) {
                val = n;
            }
        }

        va_end(args);

        return val;
    }
} // namespace n301

auto main() -> int {
    {
        using namespace n301;

        std::println("min(42, 7)= {}", min(2, 42, 7));
        std::println("min(1,5,3,-4,9)= {}", min(5, 1, 5, 3, -4, 9));
    }
    return 0;
}
