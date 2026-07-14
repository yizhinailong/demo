import std;

namespace n903 {
    template <typename R>
    struct replicate_iterator;

    template <typename R>
    struct replicate_sentinel
    {
        using base = std::ranges::iterator_t<R>;
        using size_type = std::ranges::range_difference_t<R>;

        replicate_sentinel() = default;

        constexpr replicate_sentinel(base end) : end_{ end } {}
        constexpr bool is_at_end(replicate_iterator<R> it) const;

    private:
        base      end_;
    };

    template <typename R>
    struct replicate_iterator : std::ranges::iterator_t<R>
    {
        using base = std::ranges::iterator_t<R>;
        using value_type = typename std::ranges::range_value_t<R>;
        using reference_type = typename std::ranges::range_reference_t<R>;

        constexpr replicate_iterator(base start, std::ranges::range_difference_t<R> count) :
            pos_{ start }, count_{ count }
        {
        }

        constexpr replicate_iterator operator++(int)
        {
            if (step_ == count_)
            {
                step_ = 1;
                pos_++;
            }
            else
            {
                step_++;
            }

            return pos_;
        }

        constexpr replicate_iterator& operator++()
        {
            if (step_ == count_)
            {
                step_ = 1;
                pos_++;
            }
            else
            {
                step_++;
            }

            return (*this);
        }

        constexpr reference_type operator*() const
        {
            return *pos_;
        }

        constexpr bool operator==(replicate_sentinel<R> s) const
        {
            return s.is_at_end(*this);
        }

        constexpr base const value() const { return pos_; }

    private:
        base                                pos_;
        std::ranges::range_difference_t<R>  count_;
        std::ranges::range_difference_t<R>  step_ = 1;
    };

    template <typename R>
    constexpr bool replicate_sentinel<R>::is_at_end(replicate_iterator<R> it) const
    {
        return end_ == it.value();
    }

    template<std::ranges::view R>
    struct replicate_view : public std::ranges::view_interface<replicate_view<R>>
    {
    private:
        R                                   base_;
        std::ranges::range_difference_t<R>  count_;

    public:
        replicate_view() = default;

        constexpr replicate_view(R base, std::ranges::range_difference_t<R> count)
            : base_(std::move(base))
            , count_(count)
        {
        }

        constexpr R base() const&
            requires std::copy_constructible<R>
        { return base_; }
        constexpr R base()&& { return std::move(base_); }

        constexpr std::ranges::range_difference_t<R> const& increment() const { return count_; }

        constexpr auto begin()
        {
            return replicate_iterator<R const>(std::ranges::begin(base_), count_);
        }

        constexpr auto begin() const requires std::ranges::range<R const>
        {
            return replicate_iterator<R const>(std::ranges::begin(base_), count_);
        }

        constexpr auto end()
        {
            return replicate_sentinel<R const>{std::ranges::end(base_)};
        }

        constexpr auto end() const requires std::ranges::range<R const>
        {
            return replicate_sentinel<R const>{std::ranges::end(base_)};
        }

        constexpr auto size() const requires std::ranges::sized_range<R const>
        { return count_ * std::ranges::size(base_); }

        constexpr auto size() requires std::ranges::sized_range<R>
        { return count_ * std::ranges::size(base_); }
    };

    template<class R>
    replicate_view(R&& base, std::ranges::range_difference_t<R> count)
        ->replicate_view<std::ranges::views::all_t<R>>;

    namespace details
    {
        using test_range_t = std::ranges::views::all_t<std::vector<int>>;
        static_assert(std::input_iterator<replicate_iterator<test_range_t>>);
        static_assert(std::sentinel_for<replicate_sentinel<test_range_t>, replicate_iterator<test_range_t>>);

        struct replicate_view_fn_closure
        {
            std::size_t step_;
            constexpr replicate_view_fn_closure(std::size_t step)
                : step_(step)
            {
            }

            template <std::ranges::range R>
            constexpr auto operator()(R&& r) const
            {
                return replicate_view(std::forward<R>(r), step_);
            }
        };

        struct replicate_view_fn
        {
            template<std::ranges::range R>
            constexpr auto operator () (R&& r, std::size_t step) const
            {
                return replicate_view(std::forward<R>(r), step);
            }

            constexpr auto operator () (std::size_t step) const
            {
                return replicate_view_fn_closure(step);
            }
        };

        template <std::ranges::range R>
        constexpr auto operator | (R&& r, replicate_view_fn_closure&& a)
        {
            return std::forward<replicate_view_fn_closure>(a)(std::forward<R>(r));
        }
    }

    namespace views
    {
        inline constexpr details::replicate_view_fn replicate;
    }
} // namespace n903

auto main() -> int {
    {
        using namespace n903;
        
        std::cout << "replicate(2)\n";
        for (auto i : std::views::iota(1, 5) | n903::views::replicate(2))
           std::cout << i << '\n';
        
        std::cout << "replicate(2)\n";
        for (auto i : std::views::iota(1, 5) | n903::views::replicate(2) | std::views::take(5))
           std::cout << i << '\n';
    }
    return 0;
}
