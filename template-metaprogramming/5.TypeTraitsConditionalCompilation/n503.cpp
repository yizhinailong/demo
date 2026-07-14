import std;

namespace n503 {
    template <typename T>
    auto begin(T& c) { return c.begin(); }

    template <typename T, size_t N>
    T* begin(T(&arr)[N]) {return arr; }

    template <typename T>
    void increment(T& val) { val++; }

    template <typename T, size_t N>
    void handle(T(&arr)[N], char(*)[N % 2 == 0] = 0)
    {
        std::cout << "handle even array\n";
    }

    template <typename T, size_t N>
    void handle(T(&arr)[N], char(*)[N % 2 == 1] = 0)
    {
        std::cout << "handle odd array\n";
    }
} // namespace n503

auto main() -> int {
    {
        std::array<int, 5> arr1{ 1,2,3,4,5 };
        std::cout << *n503::begin(arr1) << '\n';
        
        int arr2[]{ 5,4,3,2,1 };
        std::cout << *n503::begin(arr2) << '\n';
    }

    {
        int a = 42;
        n503::increment(a);  // OK
        
        std::string s{ "42" };
        // n503::increment(s);  // error
    }

    {
        int arr1[]{ 1,2,3,4,5 };
        n503::handle(arr1);
        
        int arr2[]{ 1,2,3,4 };
        n503::handle(arr2);
    }
    return 0;
}
