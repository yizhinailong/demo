import std;

namespace n446 {
    struct dictionary_traits
    {
        using key_type = int;
        using map_type = std::map<key_type, std::string>;
        static constexpr int identity = 1;
    };

    template <typename T>
    struct dictionary : T::map_type
    {
        int start_key{ T::identity };
        T::key_type next_key;

        using value_type = T::map_type::mapped_type;

        void add(T::key_type const&, value_type const&) {}
    };
} // namespace n446

auto main() -> int {
    {
        using namespace n446;
        dictionary<dictionary_traits> d;
        d.add(1, "2");
    }
    return 0;
}
