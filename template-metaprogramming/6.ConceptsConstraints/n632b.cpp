import std;

namespace n632b {
    void handle(long v)
    {
        if constexpr (sizeof(long) > sizeof(int))
        {
            /* do something else */
        }
        else
        {
            /* do something */
        }
    }
} // namespace n632b

auto main() -> int {
    return 0;
}
