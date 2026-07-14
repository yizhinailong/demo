import std;

namespace n711a {
    namespace details 
    {
        template <typename Iter, typename Distance>
        void advance(Iter& it, Distance n, std::random_access_iterator_tag)
        {
            it += n;
        }

        template <typename Iter, typename Distance>
        void advance(Iter& it, Distance n, std::bidirectional_iterator_tag)
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

        template <typename Iter, typename Distance>
        void advance(Iter& it, Distance n, std::input_iterator_tag)
        {
            while (n--)
            {
                ++it;
            }
        }
    }

    template <typename Iter, typename Distance>
    void advance(Iter& it, Distance n)
    {
        details::advance(it, n,
            typename std::iterator_traits<Iter>::iterator_category{});
    }
} // namespace n711a

auto main() -> int {
    {
        std::vector<int> v{ 1,2,3,4,5 };
        auto sv = std::begin(v);
        n711a::advance(sv, 2);
        
        std::list<int> l{ 1,2,3,4,5 };
        auto sl = std::begin(l);
        n711a::advance(sl, 2);
    }
    return 0;
}
