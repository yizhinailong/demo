import std;

namespace n712c {
    struct knight
    {
        void attack() { std::cout << "draw sword\n"; }
    };

    struct mage
    {
        void attack() { std::cout << "spell magic curse\n"; }
    };

    struct game
    {
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

        template <typename T>
        void addUnit(T& unit)
        {
            units.push_back(std::make_unique<game_unit_wrapper<T>>(unit));
        }

        void fight()
        {
            for (auto& u : units)
                u->attack();
        }
    private:
        std::vector<std::unique_ptr<game_unit>> units;
    };
} // namespace n712c

auto main() -> int {
    {
        using namespace n712c;
        
        knight k;
        mage m;
        
        game g;
        g.addUnit(k);
        g.addUnit(m);
        
        g.fight();
    }
    return 0;
}
