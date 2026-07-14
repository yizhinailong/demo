import std;

namespace n636c {
    auto add(std::integral auto ... args)
    {
        return (args + ...);
    }
} // namespace n636c

auto main() -> int {
    {
        using namespace n636c;
        
        add(1, 2, 3);
    }
    return 0;
}
