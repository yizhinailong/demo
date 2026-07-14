import std;

namespace n633b {
    template <std::integral T>
    struct wrapper
    {
        T value;
    };

    template <std::integral T>
        requires (sizeof(T) == 4)
    struct wrapper<T>
    {
        union
        {
            T value;
            struct
            {
                std::uint8_t byte4;
                std::uint8_t byte3;
                std::uint8_t byte2;
                std::uint8_t byte1;
            };
        };
    };
} // namespace n633b

auto main() -> int {
    {
        using namespace n633b;
        
        wrapper<short> a{ 42 };
        std::cout << a.value << '\n';
        
        wrapper<int> b{ 0x11223344 };
        std::cout << std::hex << b.value << '\n';
        std::cout << std::hex << (int)b.byte1 << '\n';
        std::cout << std::hex << (int)b.byte2 << '\n';
        std::cout << std::hex << (int)b.byte3 << '\n';
        std::cout << std::hex << (int)b.byte4 << '\n';
    }
    return 0;
}
