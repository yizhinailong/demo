import std;

namespace n606 {
    template<typename T>
    concept arithmetic = requires { std::is_arithmetic_v<T>; };

    template <arithmetic T>
    T add(T const a, T const b) 
    {
        return a + b;
    }
} // namespace n606

auto main() -> int {
    {
        using namespace n606;
        using namespace std::string_literals;
        
        add(42, 1);
        add(42.0, 1.0);
        //add("42"s, "1"s); // error: the associated constraints are not satisfied
        //add("42", "1");   // error: the associated constraints are not satisfied
    }
    return 0;
}
