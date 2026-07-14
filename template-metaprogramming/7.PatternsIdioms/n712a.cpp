import std;

namespace n712a {
    struct knight
    {
        void attack() { std::cout << "draw sword\n"; }
    };

    struct mage
    {
        void attack() { std::cout << "spell magic curse\n"; }
    };

    struct game_unit
    {
        virtual void attack() = 0;
        virtual ~game_unit() = default;
    };

    struct knight_unit : game_unit
    {
        knight_unit(knight& u) : k(u) {}
        void attack() override { k.attack(); }

    private:
        knight& k;
    };

    struct mage_unit : game_unit
    {
        mage_unit(mage& u) : m(u) {}
        void attack() override { m.attack(); }

    private:
        mage& m;
    };

    void fight(std::vector<game_unit*> const & units)
    {
        for (auto u : units)
            u->attack();
    }
} // namespace n712a

auto main() -> int {
    {
        using namespace n712a;
        
        knight k;
        mage m;
        
        knight_unit ku{ k };
        mage_unit mu{ m };
        
        std::vector<game_unit*> v{ &ku, &mu };
        fight(v);
    }
    return 0;
}
