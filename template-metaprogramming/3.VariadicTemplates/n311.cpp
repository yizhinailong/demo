import std;

namespace n311 {
    template <typename... Ts, typename... Us>
    constexpr auto multipacks(Ts... args1, Us... args2) -> void {
        static_assert(sizeof...(args1) == sizeof...(args2), "Packs must be of equal sizes.");
    }
} // namespace n311

auto main() -> int {
    {
        using namespace n311;

        // multipacks<int>(1, 2, 3, 4, 5, 6); // error
        // multipacks<int, int>(1, 2, 3, 4, 5, 6); // error
        multipacks<int, int, int>(1, 2, 3, 4, 5, 6);

        // multipacks<int>(1, 2, 3, 4.0, 5.0, 6.0); // error
        multipacks<int, int, int>(1, 2, 3, 4.0, 5.0, 6.0);
    }
    return 0;
}
