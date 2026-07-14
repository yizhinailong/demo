import std;

namespace n501 {
    template <typename T>
    struct is_floating_point
    {
        static const bool value = false;
    };

    template <>
    struct is_floating_point<float>
    {
        static const bool value = true;
    };

    template <>
    struct is_floating_point<double>
    {
        static const bool value = true;
    };

    template <>
    struct is_floating_point<long double>
    {
        static const bool value = true;
    };

    template <typename T>
    void process_real_number(T const value)
    {
        static_assert(is_floating_point<T>::value);

        std::cout << "processing a real number: " << value << '\n';
    }
} // namespace n501

auto main() -> int {
    {
        using namespace n501;
        
        static_assert(is_floating_point<float>::value);
        static_assert(is_floating_point<double>::value);
        static_assert(is_floating_point<long double>::value);
        static_assert(!is_floating_point<int>::value);
        static_assert(!is_floating_point<bool>::value);
        
        process_real_number(42.0);
        //process_real_number(42); // error: static assertion failed
    }
    return 0;
}
