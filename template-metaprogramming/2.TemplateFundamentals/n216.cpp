import std;

namespace n216 {
    template <typename T>
    class simple_wrapper {
    public:
        T value;
    };

    template <typename T>
    class fancy_warpper {
    public:
        explicit fancy_warpper(T const value)
            : m_value(value) {}

        auto get() const -> T const& {
            return m_value;
        }

    private:
        T m_value;
    };

    template <typename T, typename U, template <typename> typename W = fancy_warpper>
    class wrapper_pair {
    public:
        explicit wrapper_pair(T const a, U const b)
            : item1(a), item2(b) {}

        W<T> item1;
        W<U> item2;
    };
} // namespace n216

auto main() -> int {
    {
        using namespace n216;

        wrapper_pair<int, int> p1(41, 42);
        std::println("p1.item1.get() = {}", p1.item1.get());
        std::println("p1.item2.get() = {}", p1.item2.get());

        wrapper_pair<int, int, simple_wrapper> p2(41, 42);
        std::println("p2.item1.value = {}", p2.item1.value);
        std::println("p2.item2.value = {}", p2.item2.value);
    }
    return 0;
}
