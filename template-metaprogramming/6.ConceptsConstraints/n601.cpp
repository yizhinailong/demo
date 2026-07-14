import std;

namespace n601 {
    template <typename T>
    T add(T const a, T const b)
    {
        return a + b;
    }
} // namespace n601

auto main() -> int {
    {
        using namespace n601;
        using namespace std::string_literals;
        
        add(42, 1);
        add(42.0, 1.0);
        add("42"s, "1"s);
        //add("42", "1");   // error: cannot add two pointers
    }
    return 0;
}
