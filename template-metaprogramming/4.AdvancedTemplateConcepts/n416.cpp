import std;

namespace n416 {
    template <size_t Size>
    void process1(int a[Size]) {
        std::cout << "process(int[Size])" << '\n';
    }

    template <size_t Size>
    void process2(int a[5][Size]) {
        std::cout << "process(int[5][Size])" << '\n';
    }

    template <size_t Size>
    void process3(int (&a)[Size]) {
        std::cout << "process(int[Size]&)" << '\n';
    }

    template <size_t Size>
    void process4(int (*a)[Size]) {
        std::cout << "process(int[Size]*)" << '\n';
    }
} // namespace n416

auto main() -> int {
    {
        using namespace n416;

        int arr1[10];
        int arr2[5][10];

        // process1(arr1);   // error
        process2(arr2);  // OK
        process3(arr1);  // OK
        process4(&arr1); // OK
    }
    return 0;
}
