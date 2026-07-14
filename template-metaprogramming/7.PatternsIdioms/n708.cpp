import std;

namespace n708 {
    template <typename T>
    struct base_unit
    {
        template <typename U>
        void ally_with(U& other);
    };

    struct hero : base_unit<hero>
    {
        hero(std::string_view n) : name(n) {}

        hero* begin() { return this; }
        hero* end() { return this + 1; }

    private:
        std::string name;
        std::set<hero*> connections;

        template <typename U>
        friend struct base_unit;

        template <typename U>
        friend std::ostream& operator<<(std::ostream& os, base_unit<U>& object);
    };

    struct hero_party : std::vector<hero>, base_unit<hero_party>
    {};

    template <typename T>
    template <typename U>
    void base_unit<T>::ally_with(U& other)
    {
        for (hero& from : *static_cast<T*>(this))
        {
            for (hero& to : other)
            {
                from.connections.insert(&to);
                to.connections.insert(&from);
            }
        }
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& os, base_unit<T>& object)
    {
        for (hero& obj : *static_cast<T*>(&object))
        {
            for (hero* n : obj.connections)
                os << obj.name << " --> [" << n->name << "]" << '\n';
        }
        return os;
    }
} // namespace n708

auto main() -> int {
    {
        using namespace n708;
        
        hero k1("Arthur");
        hero k2("Sir Lancelot");
        
        hero_party p1;
        p1.emplace_back("Bors");
        
        hero_party p2;
        p2.emplace_back("Cador");
        p2.emplace_back("Constantine");
        
        k1.ally_with(k2);
        k1.ally_with(p1);
        
        p1.ally_with(k2);
        p1.ally_with(p2);
        
        std::cout << k1 << '\n';
        std::cout << k2 << '\n';
        std::cout << p1 << '\n';
        std::cout << p2 << '\n';
    }
    return 0;
}
