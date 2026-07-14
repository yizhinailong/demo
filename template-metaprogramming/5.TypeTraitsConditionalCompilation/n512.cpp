import std;

template <class T>
constexpr bool always_false = std::false_type::value;

namespace n512 {
    template <typename T>
    void f()
    {
        if constexpr (std::is_arithmetic_v<T>)
        {

        }
        else
            static_assert(always_false<T>, "Must be arithmetic"); // ill-formed: invalid for every T
    }
} // namespace n512

auto main() -> int {
    {
        using namespace n512;
        f<int>();
        f<double>();
        //f<n502::widget>();
    }
    return 0;
}
