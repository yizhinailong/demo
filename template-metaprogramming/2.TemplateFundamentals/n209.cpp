import std;

namespace n209 {
    template <typename T, std::size_t S>
    class buffer {
    public:
        auto data() const -> T const* {
            return m_data;
        }

        constexpr auto operator[](std::size_t const index) -> T& {
            return m_data[index];
        }

        constexpr auto operator[](std::size_t const index) const -> T const& {
            return m_data[index];
        }

    private:
        T m_data[S];
    };

    template <typename T, std::size_t S>
    auto make_buffer() -> buffer<T, S> {
        return {};
    }
} // namespace n209

auto main() -> int {
    {
        using namespace n209;

        buffer<int, 10> a1;
        a1[0] = 10;
        std::println("a[0] = {}", a1[0]);

        auto a2 = make_buffer<int, 10>();
    }

    {
        using namespace n209;

        buffer<int, 10>    a1;
        buffer<int, 5 * 2> a2;

        std::println(
            "std::is_same_v<buffer<int, 10>, buffer<int, 5 * 2>> = {}",
            std::is_same_v<buffer<int, 10>, buffer<int, 5 * 2>>
        );
        std::print(
            "std::is_same_v<decltype(a1), decltype(a2)> = {}",
            std::is_same_v<decltype(a1), decltype(a2)>
        );

        static_assert(std::is_same_v<decltype(a1), decltype(a2)>);

        buffer<int, 3 * 2> a3;
        static_assert(!std::is_same_v<decltype(a1), decltype(a3)>);
    }
    return 0;
}
