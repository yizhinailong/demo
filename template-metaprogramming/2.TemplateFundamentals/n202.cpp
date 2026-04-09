import std;

namespace n202 {

    template <typename T>
    class wrapper {
    public:
        explicit wrapper(T const value)
            : m_value(value) {}

        auto get() const -> T const& {
            return m_value;
        }

    private:
        T m_value;
    };

} // namespace n202

auto main() -> int {
    {
        using namespace n202;

        wrapper         a(42);   // wrapper int
        wrapper<int>    b(42);   // wrapper int
        wrapper<short>  c(42);   // wrapper short
        wrapper<double> d(42.5); // wrapper double
        wrapper         e("42"); // wrapper const char *
    }
    return 0;
}
