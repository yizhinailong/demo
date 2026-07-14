import std;

namespace n440 {
    struct wrapper;

    template <typename T>
    void print(wrapper const& w);

    template <typename T>
    struct printer;

    struct wrapper
    {
        wrapper(int const v) :value(v)
        {
        }
    private:
        int value;

        friend void print<int>(wrapper const&);
        friend struct printer<int>;
    };

    template <typename T>
    void print(wrapper const& w)
    {
        // std::cout << w.value << '\n'; // error
    }

    template <>
    void print<int>(wrapper const& w)
    {
        std::cout << w.value << '\n';
    }

    template <typename T>
    struct printer
    {
        void operator()(wrapper const& w)
        {
            // std::cout << w.value << '\n'; // error
        }
    };

    template <>
    struct printer<int>
    {
        void operator()(wrapper const& w)
        {
            std::cout << w.value << '\n';
        }
    };
} // namespace n440

auto main() -> int {
    {
        using namespace n440;
        
        wrapper w{ 43 };
        print<int>(w);
        print<char>(w);
        printer<int>()(w);
        printer<double>()(w);
    }
    return 0;
}
