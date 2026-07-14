import std;

namespace n712b {
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

    template <typename T>
    struct game_unit_wrapper : public game_unit
    {
        game_unit_wrapper(T& unit) : t(unit) {}

        void attack() override { t.attack(); }
    private:
        T& t;
    };

    void fight(std::vector<game_unit*> const& units)
    {
        for (auto u : units)
            u->attack();
    }
} // namespace n712b

auto main() -> int {
    {
        using namespace n712b;
        
        knight k;
        mage m;
        
        game_unit_wrapper ku{ k };
        game_unit_wrapper mu{ m };
        
        std::vector<game_unit*> v{ &ku, &mu };
        fight(v);
    }
    return 0;
}
