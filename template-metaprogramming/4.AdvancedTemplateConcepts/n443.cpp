import std;

namespace n443 {
    template <typename T>
    struct printer;

    template <typename T>
    struct wrapper
    {
        wrapper(T const v) :value(v) {}
    private:
        T value;

        template <typename U>
        friend void print(wrapper<U> const&);

        template <typename U>
        friend struct printer;
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
} // namespace n443

auto main() -> int {
    {
        using namespace n443;
        
        wrapper w1{ 43 };
        print(w1);
        printer<int>()(w1);
        
        wrapper w2{ 'a' };
        print(w2);
        printer<char>()(w2);
    }
    return 0;
}
