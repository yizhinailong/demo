import std;

namespace n204 {
    template <typename T>
    class composition {
    public:
        auto add(T const a, T const b) -> T const {
            return a + b;
        }
    };
} // namespace n204

auto main() -> int {
    {
        using namespace n204;

        composition<int> a;
        a.add(10, 20);
    }
    return 0;
}
