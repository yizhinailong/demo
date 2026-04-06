import std;

namespace n102 {
    template <typename T>
    T max(T const a, T const b) {
        return a > b ? a : b;
    }

    struct foo {};

    template <typename T>
    void swap(T* a, T* b) {
        T t = *a;
        *a  = *b;
        *b  = t;
    }

    template <typename T>
    int partition(T arr[], int const low, int const high) {
        T   pivot = arr[high];
        int i     = (low - 1);

        for (int j = low; j <= high - 1; j++) {
            if (arr[j] < pivot) {
                i++;
                swap(&arr[i], &arr[j]);
            }
        }

        swap(&arr[i + 1], &arr[high]);

        return i + 1;
    }

    template <typename T>
    void quicksort(T arr[], int const low, int const high) {
        if (low < high) {
            int const pi = partition(arr, low, high);
            quicksort(arr, low, pi - 1);
            quicksort(arr, pi + 1, high);
        }
    }

    template <typename T>
    struct vector {
        vector();

        std::size_t size() const;
        std::size_t capacity() const;
        bool        empty() const;

        void clear();
        void resize(std::size_t const size);

        void push_back(T value);
        void pop_back();

        T at(std::size_t const index) const;
        T operator[](std::size_t const index) const;

    private:
        T*          m_data;
        std::size_t m_size;
        std::size_t m_capacity;
    };

    template <typename T>
    constexpr T NewLine = T('\n');
} // namespace n102

int main() {
    {
        using namespace n102;

        max(1, 2);     // OK, compares ints
        max(1.0, 2.0); // OK, compares doubles

        // foo f1, f2;
        // max(f1, f2);      // Error, operator> not overloaded for foo

        max<int>(1, 2);
        max<double>(1.0, 2.0);
        // max<foo>(f1, f2);
    }

    {
        using namespace n102;

        int arr[] = { 13, 1, 8, 3, 5, 2, 1 };
        int n     = sizeof(arr) / sizeof(arr[0]);
        quicksort(arr, 0, n - 1);
    }

    {
        using namespace n102;

        std::wstring test  = L"demo";
        test              += NewLine<wchar_t>;
        std::wcout << test;
    }

    return 0;
}
