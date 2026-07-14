import std;

namespace n605 {
    template <typename T>      
    T add(T const a, T const b) requires std::is_arithmetic_v<T>
    {
        return a + b;
    }
} // namespace n605

auto main() -> int {
    {
        using namespace n605;
        using namespace std::string_literals;
        
        add(42, 1);
        add(42.0, 1.0);
        //add("42"s, "1"s); // error: the associated constraints are not satisfied
        //add("42", "1");   // error: the associated constraints are not satisfied
    }
    return 0;
}
