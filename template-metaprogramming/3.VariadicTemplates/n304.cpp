import std;

namespace n304 {
    int min(int a, int b) {
        return a < b ? a : b;
    }

    int min(int a, int b, int c) {
        return min(a, min(b, c));
    }

    int min(int a, int b, int c, int d) {
        return min(a, min(b, min(c, d)));
    }

    int min(int a, int b, int c, int d, int e) {
        return min(a, min(b, min(c, min(d, e))));
    }
} // namespace n304

auto main() -> int {
    {
        using namespace n304;

        std::println("min(2, 42, 7) = {}", min(2, 42, 7));
        std::println("min(5, 1, 5, 3, -4) = {}", min(5, 1, 5, 3, -4));
    }
    return 0;
}
