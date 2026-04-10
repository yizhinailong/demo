import std;

namespace n206 {
    template <typename T>
    class wrapper {
    public:
        explicit wrapper(T const value)
            : m_value(value) {}

        auto get() const -> T const& {
            return m_value;
        }

        template <typename U>
        auto as() const -> U {
            return static_cast<U>(m_value);
        }

    private:
        T m_value;
    };
} // namespace n206

auto main() -> int {
    {
        using namespace n206;

        wrapper a(10.5);

        auto b = a.get();
        auto c = a.as<int>();

        std::println("b = {}, c = {}", b, c);
    }
    return 0;
}
