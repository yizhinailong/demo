import std;

namespace n902 {
    template <typename R>
    struct step_iterator;

    template <typename R>
    struct step_sentinel
    {
        using base = std::ranges::iterator_t<R>;
        using size_type = std::ranges::range_difference_t<R>;

        step_sentinel() = default;

        constexpr step_sentinel(base end) : end_{ end } {}
        constexpr bool is_at_end(step_iterator<R> it) const;

    private:
        base      end_;
    };

    template <typename R>
    struct step_iterator : std::ranges::iterator_t<R>
    {
        using base = std::ranges::iterator_t<R>;
        using value_type = typename std::ranges::range_value_t<R>;
        using reference_type = typename std::ranges::range_reference_t<R>;

        constexpr step_iterator(base start, base end, std::ranges::range_difference_t<R> step) :
            pos_{ start }, end_{ end }, step_{ step }
        {
        }

        constexpr step_iterator operator++(int)
        {
            auto ret = *this;
            pos_ = std::ranges::next(pos_, step_, end_);
            return ret;
        }

        constexpr step_iterator& operator++()
        {
            pos_ = std::ranges::next(pos_, step_, end_);
            return *this;
        }

        constexpr reference_type operator*() const
        {
            return *pos_;
        }

        constexpr bool operator==(step_sentinel<R> s) const
        {
            return s.is_at_end(*this);
        }

        constexpr base const value() const { return pos_; }

    private:
        base                                pos_;
        base                                end_;
        std::ranges::range_difference_t<R>  step_;
    };

    template <typename R>
    constexpr bool step_sentinel<R>::is_at_end(step_iterator<R> it) const
    {
        return end_ == it.value();
    }

    template<std::ranges::view R>
    struct step_view : public std::ranges::view_interface<step_view<R>>
    {
    private:
        R                                   base_;
        std::ranges::range_difference_t<R>  step_;

    public:
        step_view() = default;

        constexpr step_view(R base, std::ranges::range_difference_t<R> step)
            : base_(std::move(base))
            , step_(step)
        {
        }

        constexpr R base() const&
            requires std::copy_constructible<R>
        { return base_; }
        constexpr R base()&& { return std::move(base_); }

        constexpr std::ranges::range_difference_t<R> const& increment() const { return step_; }

        constexpr auto begin()
        {
            return step_iterator<R const>(std::ranges::begin(base_), std::ranges::end(base_), step_);
        }

        constexpr auto begin() const requires std::ranges::range<R const>
        {
            return step_iterator<R const>(std::ranges::begin(base_), std::ranges::end(base_), step_);
        }

        constexpr auto end()
        {
            return step_sentinel<R const>{std::ranges::end(base_)};
        }

        constexpr auto end() const requires std::ranges::range<R const>
        {
            return step_sentinel<R const>{std::ranges::end(base_)};
        }

        constexpr auto size() const requires std::ranges::sized_range<R const>
        { auto d = std::ranges::size(base_); return step_ == 1 ? d : static_cast<int>((d + 1) / step_); }

        constexpr auto size() requires std::ranges::sized_range<R>
        { auto d = std::ranges::size(base_); return step_ == 1 ? d : static_cast<int>((d + 1) / step_); }
    };

    template<class R>
    step_view(R&& base, std::ranges::range_difference_t<R> step)
        ->step_view<std::ranges::views::all_t<R>>;

    namespace details
    {
        using test_range_t = std::ranges::views::all_t<std::vector<int>>;
        static_assert(std::input_iterator<step_iterator<test_range_t>>);
        static_assert(std::sentinel_for<step_sentinel<test_range_t>, step_iterator<test_range_t>>);

        struct step_view_fn_closure
        {
            std::size_t step_;
            constexpr step_view_fn_closure(std::size_t step)
                : step_(step)
            {
            }

            template <std::ranges::range R>
            constexpr auto operator()(R&& r) const
            {
                return step_view(std::forward<R>(r), step_);
            }
        };

        struct step_view_fn
        {
            template<std::ranges::range R>
            constexpr auto operator () (R&& r, std::size_t step) const
            {
                return step_view(std::forward<R>(r), step);
            }

            constexpr auto operator () (std::size_t step) const
            {
                return step_view_fn_closure(step);
            }
        };

        template <std::ranges::range R>
        constexpr auto operator | (R&& r, step_view_fn_closure&& a)
        {
            return std::forward<step_view_fn_closure>(a)(std::forward<R>(r));
        }
    }

    namespace views
    {
        inline constexpr details::step_view_fn step;
    }
} // namespace n902

auto main() -> int {
    {
        using namespace n902;
        
        std::cout << "step(1)\n";
        for (auto i : std::views::iota(1, 10) | n902::views::step(1))
           std::cout << i << '\n';
        
        std::cout << "step(2)\n";
        for (auto i : std::views::iota(1, 10) | n902::views::step(2))
           std::cout << i << '\n';
        
        std::cout << "step(3)\n";
        for (auto i : std::views::iota(1, 10) | n902::views::step(3))
           std::cout << i << '\n';
        
        std::cout << "step(4)\n";
        for (auto i : std::views::iota(1, 10) | n902::views::step(4))
           std::cout << i << '\n';
        
        std::cout << "step(5)\n";
        for (auto i : std::views::iota(1, 10) | n902::views::step(5))
           std::cout << i << '\n';
        
        std::cout << "step(2) | take(3)\n";
        for (auto i : std::views::iota(1, 10) | 
                      n902::views::step(2) |
                      std::views::take(3))
           std::cout << i << '\n';
        
        std::cout << "step(2) | take(3)\n";
        auto r = n902::views::step(std::views::iota(1, 10), 2);
        auto t = std::ranges::take_view(r, 3);
        for (auto i : t)
           std::cout << i << '\n';
        
    }

    {
        using namespace n902;
        
        auto v = std::views::iota(1, 10) | n902::views::step(1);
        auto it = v.begin();
        it++; // post-increment
        ++it; // pre-increment
    }
    return 0;
}
