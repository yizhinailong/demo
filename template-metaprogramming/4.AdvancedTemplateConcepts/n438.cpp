import std;

namespace n438 {
    struct wrapper
    {   
        wrapper(int const v) :value(v)
        {}
    private:
        int value;

        friend void print(wrapper const & w);
    };

    void print(wrapper const& w)
    {
        std::cout << w.value << '\n';
    }
} // namespace n438

auto main() -> int {
    {
        using namespace n438;
        
        wrapper w{ 42 };
        print(w);
    }
    return 0;
}
