import std;

namespace n101 {
    auto max(int const a, int const b) -> int {
        return a > b ? a : b;
    }

    auto max(double const a, double const b) -> double {
        return a > b ? a : b;
    }

    using swap_fn    = auto (*)(void*, int const, int const) -> void;
    using compare_fn = auto (*)(void*, int const, int const) -> bool;

    auto partition(void* arr, int const low, int const high, compare_fn fcomp, swap_fn fswap) -> int {
        int i = low - 1;

        for (int j = low; j <= high - 1; j++) {
            if (fcomp(arr, j, high)) {
                i++;
                fswap(arr, i, j);
            }
        }

        fswap(arr, i + 1, high);

        return i + 1;
    }

    auto quicksort(void* arr, int const low, int const high, compare_fn fcomp, swap_fn fswap) -> void {
        if (low < high) {
            int const pi = partition(arr, low, high, fcomp, fswap);
            quicksort(arr, low, pi - 1, fcomp, fswap);
            quicksort(arr, pi + 1, high, fcomp, fswap);
        }
    }

    auto swap_int(void* arr, int const i, int const j) -> void {
        int* iarr = (int*)arr;
        int  t    = iarr[i];
        iarr[i]   = iarr[j];
        iarr[j]   = t;
    }

    auto less_int(void* arr, int const i, int const j) -> bool {
        int* iarr = (int*)arr;
        return iarr[i] <= iarr[j];
    }

    struct int_vector {
        int_vector();

        auto size() const -> std::size_t;
        auto capacity() const -> std::size_t;
        auto empty() const -> bool;

        auto clear() -> void;
        auto resize(std::size_t const size) -> void;

        auto push_back(int value) -> void;
        auto pop_back() -> void;

        auto at(std::size_t const index) const -> int;
        auto operator[](std::size_t const index) const -> int;

    private:
        int*        m_data;
        std::size_t m_size;
        std::size_t m_capacity;
    };

    constexpr char     NewLine    = '\n';
    constexpr wchar_t  NewLineW   = L'\n';
    constexpr char8_t  NewLineU8  = u8'\n';
    constexpr char16_t NewLineU16 = u'\n';
    constexpr char32_t NewLineU32 = U'\n';
} // namespace n101

auto main() -> int {
    {
        using namespace n101;

        int arr[] = { 13, 1, 8, 3, 5, 2, 1 };
        int n     = sizeof(arr) / sizeof(arr[0]);
        quicksort(arr, 0, n - 1, less_int, swap_int);
    }

    return 0;
}
