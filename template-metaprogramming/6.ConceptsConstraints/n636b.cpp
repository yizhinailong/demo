import std;

namespace n636b {
    auto add(std::integral auto a, std::integral auto b)
    {
        return a + b;
    }
} // namespace n636b

auto main() -> int {
    {
        using namespace n636b;
        
        add(4, 2);
        //add(4.2, 0); // error
    }
    return 0;
}
