#include <cassert>

import std;

namespace n802 {
    template <typename InputIt1, typename InputIt2, typename OutputIt>
    OutputIt flatzip(
        InputIt1 first1,
        InputIt1 last1,
        InputIt2 first2,
        InputIt2 last2,
        OutputIt dest
    ) {
        auto it1 = first1;
        auto it2 = first2;

        while (it1 != last1 && it2 != last2) {
            *dest++ = *it1++;
            *dest++ = *it2++;
        }

        return dest;
    }
} // namespace n802

auto main() -> int {
    {
        using namespace n802;

        {
            std::vector<int> v1;
            std::vector<int> v2;
            std::vector<int> v3;

            flatzip(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
            assert(v3.empty());
        }

        {
            std::vector<int> v1{ 1, 2, 3 };
            std::vector<int> v2;
            std::vector<int> v3;

            flatzip(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(v3));
            assert(v3.empty());
        }

        {
            std::vector<int> v1;
            std::vector<int> v2{ 1, 2, 3 };
            std::vector<int> v3;

            flatzip(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(v3));
            assert(v3.empty());
        }

        {
            std::vector<int> v1{ 1, 2, 3 };
            std::vector<int> v2{ 4, 5 };
            std::vector<int> v3;

            flatzip(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(v3));
            assert(v3 == std::vector<int>({ 1, 4, 2, 5 }));
        }

        {
            std::vector<int> v1{ 1, 2 };
            std::vector<int> v2{ 3, 4, 5 };
            std::vector<int> v3;

            flatzip(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(v3));
            assert(v3 == std::vector<int>({ 1, 3, 2, 4 }));
        }

        {
            std::vector<int> v1{ 1, 2, 3 };
            std::vector<int> v2{ 4, 5, 6 };
            std::vector<int> v3;

            flatzip(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(v3));
            assert(v3 == std::vector<int>({ 1, 4, 2, 5, 3, 6 }));
        }
    }
    return 0;
}
