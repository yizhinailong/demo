import std;

namespace n417 {
    template <size_t N>
    struct ncube {
        static constexpr size_t dimensions = N;
    };

    template <size_t N>
    void process(ncube<N - 1> cube) {
        std::cout << cube.dimensions << '\n';
    }
} // namespace n417

auto main() -> int {
    {
        using namespace n417;

        ncube<5> cube;
        // process(cube); // error
        process<6>(cube); // OK
    }
    return 0;
}
