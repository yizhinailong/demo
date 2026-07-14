import std;

namespace n419 {
    template <typename T>
    struct point_t
    {
        point_t(T vx, T vy) : x(vx), y(vy) {}

    private:
        T x;
        T y;
    };
} // namespace n419

namespace std {
    template <typename T>
    pair(T&&, char const*) -> pair<T, std::string>;

    template <typename T>
    pair(char const*, T&&) -> pair<std::string, T>;

    pair(char const*, char const*) -> pair<std::string, std::string>;
} // namespace std

auto main() -> int {
    {
        using namespace n419;
        
        auto p = new point_t(1, 2);
        
        std::mutex mt;
        auto l = std::lock_guard(mt);
        
        // stream_t<42> s; // C++20
    }

    {
        // without custom deduction guides
        std::pair  p1{ 1, "one" };       // std::pair<int, const char*>
        std::pair  p2{ "two", 2 };       // std::pair<const char*, int>
        std::pair  p3{ "3", "three" };   // std::pair<const char*, const char*>
    }

    {
        // with custom deduction guides
        std::pair  p1{ 1, "one" };       // std::pair<int, std::string>
        std::pair  p2{ "two", 2 };       // std::pair<std::string, int>
        std::pair  p3{ "3", "three" };   // std::pair<std::string, std::string>
    }
    return 0;
}
