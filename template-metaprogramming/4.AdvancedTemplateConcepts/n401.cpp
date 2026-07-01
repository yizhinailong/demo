import std;

namespace n401 {
    template <typename T>
    struct parser;            // [1] template declaration

    void handle(double value) // [2] handle(double) definition
    {
        std::cout << "processing a double: " << value << '\n';
    }

    template <typename T>
    struct parser // [3] template definition
    {
        void parse() {
            handle(42); // [4] non-dependent name
        }
    };

    void handle(int value) // [5] handle(int) definition
    {
        std::cout << "processing an int: " << value << '\n';
    }
} // namespace n401

auto main() -> int {
    {
        using namespace n401;

        parser<int> p; // [6] template instantiation
        p.parse();
    }
    return 0;
}
