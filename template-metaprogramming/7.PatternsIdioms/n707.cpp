import std;

namespace n707 {
    struct hero
    {
        hero(std::string_view n) : name(n) {}

        void ally_with(hero& u)
        {
            connections.insert(&u);
            u.connections.insert(this);
        }
    private:
        std::string name;
        std::set<hero*> connections;

        friend std::ostream& operator<<(std::ostream& os, hero const& obj);
    };

    std::ostream& operator<<(std::ostream& os, hero const& obj)
    {
        for (hero* u : obj.connections)
            os << obj.name << " --> [" << u->name << "]" << '\n';

        return os;
    }

    struct hero_party : std::vector<hero>
    {};
} // namespace n707

auto main() -> int {
    {
        using namespace n707;
        
        hero k1("Arthur");
        hero k2("Sir Lancelot");
        hero k3("Sir Gawain");
        
        k1.ally_with(k2);
        k2.ally_with(k3);
        
        std::cout << k1 << '\n';
        std::cout << k2 << '\n';
        std::cout << k3 << '\n';
    }
    return 0;
}
