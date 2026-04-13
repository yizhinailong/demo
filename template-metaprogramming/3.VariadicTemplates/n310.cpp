import std;

namespace n310 {
    template <typename... Ts, typename... Us>
    constexpr auto multipacks(Ts... args1, Us... args2) -> void {
        std::println(
            "sizeof...(args1) = {}, sizeof...(args2) = {}",
            sizeof...(args1),
            sizeof...(args2)
        );
    }
} // namespace n310

auto main() -> int {
    {
        using namespace n310;

        multipacks<int>(1, 2, 3, 4, 5, 6);
        multipacks<int, int>(1, 2, 3, 4, 5, 6);
        multipacks<int, int, int, int, int, int>(1, 2, 3, 4, 5, 6);

        multipacks<int>(1, 2, 3, 4.0, 5.0, 6.0);
        multipacks<int, int>(1, 2, 3, 4.0, 5.0, 6.0);
    }
    return 0;
}
