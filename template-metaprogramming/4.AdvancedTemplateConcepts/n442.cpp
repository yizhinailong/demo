import std;

namespace n442 {
    template <typename T>
    struct printer;

    template <typename T>
    struct wrapper
    {
        wrapper(T const v) :value(v)
        {
        }
    private:
        T value;

        friend void print<int>(wrapper<int> const&);
        friend struct printer<int>;
    };

    template <typename T>
    void print(wrapper<T> const& w)
    {
        // std::cout << w.value << '\n'; // error
    }

    template<>
    void print(wrapper<int> const& w)
    {
        std::cout << w.value << '\n';
    }

    template <typename T>
    struct printer
    {
        void operator()(wrapper<T> const& w)
        {
            //std::cout << w.value << '\n'; // error
        }
    };

    template <>
    struct printer<int>
    {
        void operator()(wrapper<int> const& w)
        {
            std::cout << w.value << '\n';
        }
    };
} // namespace n442

auto main() -> int {
    {
        using namespace n442;
        
        wrapper w1{ 43 };
        print(w1);
        printer<int>()(w1);
        
        wrapper w2{ 'a' };
        print(w2);
        printer<char>()(w2);
    }
    return 0;
}
