import std;

namespace n502 {
    struct widget
    {
        int         id;
        std::string name;

        std::ostream& write(std::ostream& os) const
        {
            os << id << ',' << name << '\n';
            return os;
        }
    };

    struct gadget
    {
        int         id;
        std::string name;

        friend std::ostream& operator <<(std::ostream& os, gadget const& o);
    };

    std::ostream& operator <<(std::ostream& os, gadget const& o)
    {
        os << o.id << ',' << o.name << '\n';
        return os;
    }

    template <typename T>
    struct uses_write
    {
        static constexpr bool value = false;
    };

    template <>
    struct uses_write<widget>
    {
        static constexpr bool value = true;
    };

    template <typename T>
    inline constexpr bool uses_write_v = uses_write<T>::value;

    template <bool>
    struct serializer
    {
        template <typename T>
        static void serialize(std::ostream& os, T const& value)
        {
            os << value;
        }
    };

    template<>
    struct serializer<true>
    {
        template <typename T>
        static void serialize(std::ostream& os, T const& value)
        {
            value.write(os);
        }
    };

    template <typename T>
    void serialize(std::ostream& os, T const& value)
    {
        serializer<uses_write_v<T>>::serialize(os, value);
    }
} // namespace n502

namespace n507 {
    template <typename T>
    void serialize(std::ostream& os, T const& value, typename std::enable_if<n502::uses_write_v<T>>::type* = nullptr)
    {
        value.write(os);
    }

    template <typename T>
    void serialize(std::ostream& os, T const& value, typename std::enable_if<!n502::uses_write_v<T>>::type* = nullptr)
    {
        os << value;
    }
} // namespace n507

auto main() -> int {
    {
        n502::widget w{ 1, "one" };
        n502::gadget g{ 2, "two" };
        
        n507::serialize(std::cout, w);
        n507::serialize(std::cout, g);
    }
    return 0;
}
