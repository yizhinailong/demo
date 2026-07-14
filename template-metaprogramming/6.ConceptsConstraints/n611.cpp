import std;

namespace n611 {
    template <typename T>
    concept container = requires(T t)
    {
        typename T::value_type;
        typename T::size_type;
        typename T::allocator_type;
        typename T::iterator;
        typename T::const_iterator;
        t.size();
        t.begin();
        t.end();
        t.cbegin();
        t.cend();
    };

    struct foo{};

    static_assert(!container<foo>);
    static_assert(container<std::vector<foo>>);

    template <container C>
    void process(C&& c) {}
} // namespace n611

auto main() -> int {
    return 0;
}
