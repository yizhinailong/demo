import std;

namespace n617 {
    template <typename T>
    requires std::is_integral_v<T> && std::is_signed_v<T>
    T decrement(T value) 
    {
        return value--;
    }
} // namespace n617

auto main() -> int {
    return 0;
}
