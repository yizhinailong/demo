import std;

namespace n711b {
    template <std::random_access_iterator Iter, typename Distance>
    void advance(Iter& it, Distance n)
    {
        it += n;
    }

    template <std::bidirectional_iterator Iter, typename Distance>
    void advance(Iter& it, Distance n)
    {
        if (n > 0)
        {
            while (n--) ++it;
        }
        else
        {
            while (n++) --it;
        }
    }

    template <std::input_iterator Iter, typename Distance>
    void advance(Iter& it, Distance n)
    {
        while (n--)
        {
            ++it;
        }
    }
} // namespace n711b

auto main() -> int {
    {
        std::vector<int> v{ 1,2,3,4,5 };
        auto sv = std::begin(v);
        n711b::advance(sv, 2);
        
        std::list<int> l{ 1,2,3,4,5 };
        auto sl = std::begin(l);
        n711b::advance(sl, 2);
    }
    return 0;
}
