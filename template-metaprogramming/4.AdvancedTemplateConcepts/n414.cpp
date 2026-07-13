import std;

namespace n414 {
    template <typename T>
    void process(T a = 0, T b = 42) {
        std::cout << a << "," << b << '\n';
    }
} // namespace n414

auto main() -> int {
    {
        using namespace n414;
        // process();        // [1] error
        process<int>(); // [2] OK
        process(10);    // [3] OK
    }
    return 0;
}
