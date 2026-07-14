import std;

namespace n618 {
    template <typename T>
    concept Integral = std::is_integral_v<T>;

    template <typename T>
    concept Signed = std::is_signed_v<T>;

    template <typename T>
    concept SignedIntegral = Integral<T> && Signed<T>;

    template <SignedIntegral T>
    T decrement(T value)
    {
        return value--;
    }
} // namespace n618

auto main() -> int {
    return 0;
}
