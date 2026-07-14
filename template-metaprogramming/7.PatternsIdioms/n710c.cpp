import std;

namespace n710c {
    struct aggressive_style
    {
        void fight()
        {
            std::cout << "attack! attack attack!\n";
        }
    };

    struct moderate_style
    {
        void fight()
        {
            std::cout << "attack then defend\n";
        }
    };

    template <typename T>
    struct lone_warrior : T
    {
        void fight()
        {
            std::cout << "fighting alone.";
            T::fight();
        }
    };

    template <typename T>
    struct team_warrior : T
    {
        void fight()
        {
            std::cout << "fighting with a team.";
            T::fight();
        }
    };

    struct game_unit
    {
        virtual void attack() = 0;
        virtual ~game_unit() = default;
    };

    template <typename T>
    struct knight : T, game_unit
    {
        void attack()
        {
            std::cout << "draw sword.";
            T::fight();
        }
    };

    template <typename T>
    struct mage : T, game_unit
    {
        void attack()
        {
            std::cout << "spell magic curse.";
            T::fight();
        }
    };
} // namespace n710c

auto main() -> int {
    {
        using namespace n710c;
        
        std::vector<std::unique_ptr<game_unit>> units;
        
        units.emplace_back(new knight<aggressive_style>());
        units.emplace_back(new knight<moderate_style>());
        units.emplace_back(new mage<aggressive_style>());
        units.emplace_back(new mage<moderate_style>());
        units.emplace_back(new knight<lone_warrior<aggressive_style>>());
        units.emplace_back(new knight<lone_warrior<moderate_style>>());
        units.emplace_back(new knight<team_warrior<aggressive_style>>());
        units.emplace_back(new knight<team_warrior<moderate_style>>());
        units.emplace_back(new mage<lone_warrior<aggressive_style>>());
        units.emplace_back(new mage<lone_warrior<moderate_style>>());
        units.emplace_back(new mage<team_warrior<aggressive_style>>());
        units.emplace_back(new mage<team_warrior<moderate_style>>());
        
        for (auto& u : units)
           u->attack();
    }
    return 0;
}
