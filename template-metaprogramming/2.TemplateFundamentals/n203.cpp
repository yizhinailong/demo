import std;

namespace n203 {
    template <typename T>
    class wrapper;

    auto use_wrapper(wrapper<int>* ptr);
} // namespace n203

namespace n203 {
    template <typename T>
    class wrapper {
    public:
        explicit wrapper(T value)
            : m_value(value) {}

        auto get() const -> T const& {
            return m_value;
        }

    private:
        T m_value;
    };

    auto use_wrapper(wrapper<int>* ptr) {
        std::println("use_wrapper: {}", ptr->get());
    }
} // namespace n203

auto main() -> int {
    {
        using namespace n203;

        wrapper<int> a(100);
        use_wrapper(&a);
    }
    return 0;
}
