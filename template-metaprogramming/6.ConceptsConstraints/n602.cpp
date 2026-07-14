import std;

namespace n602 {
    template <typename T,
                 typename = typename std::enable_if_t<std::is_arithmetic_v<T>>>
    T add(T const a, T const b)
    {
        return a + b;
    }
} // namespace n602

auto main() -> int {
    {
        using namespace n602;
        using namespace std::string_literals;
        
        add(42, 1);
        add(42.0, 1.0);
        //add("42"s, "1"s); // error: no matching overloaded function found
        //add("42", "1");   // error: no matching overloaded function found
    }
    return 0;
}
