import std;

namespace n603 {
    template <typename T>
    T add(T const a, T const b)
    {
        static_assert(std::is_arithmetic_v<T>, "Arithmetic type required");
        return a + b;
    }
} // namespace n603

auto main() -> int {
    {
        using namespace n603;
        using namespace std::string_literals;
        
        add(42, 1);
        add(42.0, 1.0);
        //add("42"s, "1"s); // error: Arithmetic type required
        //add("42", "1");   // error: Arithmetic type required
    }
    return 0;
}
