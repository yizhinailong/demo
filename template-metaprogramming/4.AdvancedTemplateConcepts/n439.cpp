import std;

namespace n439 {
    struct wrapper
    {
        wrapper(int const v) :value(v)
        {
        }
    private:
        int value;

        template <typename T>
        friend void print(wrapper const&);

        template <typename T>
        friend struct printer;
    };   

    template <typename T>
    void print(wrapper const& w)
    {
        std::cout << w.value << '\n';
    }

    template <typename T>
    struct printer
    {
        void operator()(wrapper const& w)
        {
            std::cout << w.value << '\n';
        }
    };
} // namespace n439

auto main() -> int {
    {
        using namespace n439;
        
        wrapper w{ 42 };
        print<int>(w);
        print<char>(w);
        printer<int>()(w);
        printer<double>()(w);
    }
    return 0;
}
