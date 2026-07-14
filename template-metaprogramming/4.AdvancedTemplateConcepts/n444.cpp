import std;

namespace n444 {
    template <typename T>
    struct printer;

    template <typename T>
    struct wrapper
    {
        wrapper(T const v) :value(v) {}
    private:
        T value;

        friend void print<T>(wrapper<T> const&);
        friend struct printer<T>;
    };

    template <typename T>
    void print(wrapper<T> const& w)
    {
        std::cout << w.value << '\n';
    }

    template <typename T>
    struct printer
    {
        void operator()(wrapper<T> const& w)
        {
            std::cout << w.value << '\n';
        }
    };
} // namespace n444

auto main() -> int {
    return 0;
}
