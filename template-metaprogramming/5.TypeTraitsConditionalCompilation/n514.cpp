import std;

namespace n514 {
    struct foo
    {
        int a;
    };

    struct bar
    {
        int a = 0;
    };

    struct tar
    {
        int a = 0;
        tar() : a(0) {}
    };
} // namespace n514

auto main() -> int {
    {
        using namespace n514;
        
        std::cout << std::boolalpha;
        std::cout << std::is_trivial_v<foo> << '\n';
        std::cout << std::is_trivial_v<bar> << '\n';
        std::cout << std::is_trivial_v<tar> << '\n';
        
        std::cout << std::is_trivially_copyable_v<foo> << '\n';
        std::cout << std::is_trivially_copyable_v<bar> << '\n';
        std::cout << std::is_trivially_copyable_v<tar> << '\n';
    }
    return 0;
}
