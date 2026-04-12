import std;

namespace n235 {
    template <typename T, int S>
    struct collection {
        auto operator()() -> void {
            std::println("This is primary template.");
        }
    };

    template <typename T>
    struct collection<T, 10> {
        auto operator()() -> void {
            std::println("This is partial template <T, 10>");
        }
    };

    template <int S>
    struct collection<int, S> {
        auto operator()() -> void {
            std::println("This is partial template <int, S>");
        }
    };

    template <typename T, int S>
    struct collection<T*, S> {
        auto operator()() -> void {
            std::println("This is partial template <T*, S>");
        }
    };
} // namespace n235

auto main() -> int {
    {
        using namespace n235;

        collection<char, 100>{}();
        collection<char, 10>{}();
        collection<int, 100>{}();
        collection<char*, 100>{}();

        // collection<int, 10>{}(); // error
        // collection<char*, 10>{}(); // error
    }
    return 0;
}
