import std;

namespace n205 {
    class composition {
    public:
        template <typename T>
        auto add(T const a, T const b) -> T const {
            return a + b;
        }
    };
} // namespace n205

auto main() -> int {
    {
        using namespace n205;

        composition a;
        a.add<int>(10, 20);
    }
    return 0;
}
