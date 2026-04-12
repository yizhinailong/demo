import std;

namespace n247 {
    template <typename T>
    using customer_addresses_t = std::map<int, std::vector<T>>;

    struct delivery_address_t {};

    struct invoice_address_t {};

    using customer_delivery_addresses_t = customer_addresses_t<delivery_address_t>;
    using customer_invoice_addresses_t  = customer_addresses_t<invoice_address_t>;
} // namespace n247

namespace n247 {
    template <typename T, std::size_t S>
    struct list {
        using type = std::vector<T>;
    };

    template <typename T>
    struct list<T, 1> {
        using type = T;
    };

    template <typename T, std::size_t S>
    using list_t = typename list<T, S>::type;
} // namespace n247

auto main() -> int {
    {
        using namespace n247;
        static_assert(std::is_same_v<list_t<int, 1>, int>);
        static_assert(std::is_same_v<list_t<int, 2>, std::vector<int>>);
    }
    return 0;
}
