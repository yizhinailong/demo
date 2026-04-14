import std;

namespace n315 {
    template <typename... T>
    int suml(T... args) {
        return (... + args);
    }

    template <typename... T>
    int sumr(T... args) {
        return (args + ...);
    }

    template <typename... T>
    void printl(T... args) {
        (..., (std::cout << args)) << '\n';
    }

    template <typename... T>
    void printr(T... args) {
        ((std::cout << args), ...) << '\n';
    }

    template <typename... T>
    void print(T... args) {
        (std::cout << ... << args) << '\n';
    }

    template <typename T, typename... Args>
    void push_back_many(std::vector<T>& v, Args&&... args) {
        (v.push_back(args), ...);
    }
} // namespace n315

auto main() -> int {
    {
        using namespace n315;

        printl('d', 'o', 'g'); // dog
        printr('d', 'o', 'g'); // dog
        print('d', 'o', 'g');  // dog
    }

    {
        using namespace n315;

        std::vector<int> v;
        push_back_many(v, 1, 2, 3, 4, 5);
    }
    return 0;
}
