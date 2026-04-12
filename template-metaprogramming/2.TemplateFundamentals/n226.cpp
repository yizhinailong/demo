import std;

namespace n226 {
    template <typename T>
    struct wrapper {
        T value;
    };

    template struct wrapper<int>;

} // namespace n226

template struct n226::wrapper<double>;

namespace n226 {
    template <typename T>
    auto add(T const a, T const b) -> T {
        return a + b;
    }

    template int add(int, int);
} // namespace n226

template double n226::add(double, double);

auto main() -> int {
    return 0;
}
