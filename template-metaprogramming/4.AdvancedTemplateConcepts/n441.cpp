import std;

namespace n441 {
    template <typename T>
    struct wrapper
    {
        wrapper(T const v) :value(v)
        {
        }
    private:
        T value;

        friend void print(wrapper<int> const&);
    };

    void print(wrapper<int> const& w)
    {
        std::cout << w.value << '\n';
    }

    void print(wrapper<char> const& w)
    {
        // std::cout << w.value << '\n'; // error
    }
} // namespace n441

auto main() -> int {
    return 0;
}
