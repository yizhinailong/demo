import std;

namespace n613 {
    template <typename T>
    concept KVP = requires {
        typename T::key_type;
        typename T::value_type;
    };

    template <typename T, typename V>
    struct key_value_pair
    {
        using key_type = T;
        using value_type = V;

        key_type    key;
        value_type  value;
    };

    static_assert(KVP<key_value_pair<int, std::string>>);
    static_assert(!KVP<std::pair<int, std::string>>);

    template <typename T>
    requires std::is_arithmetic_v<T>
    struct container
    { /* ... */ };

    template <typename T>
    concept containerizeable = requires {
        typename container<T>;
    };

    static_assert(containerizeable<int>);
    static_assert(!containerizeable<std::string>);
} // namespace n613

auto main() -> int {
    return 0;
}
