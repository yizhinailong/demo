import std;

namespace n712e {
    struct knight
    {
        void attack() { std::cout << "draw sword\n"; }
    };

    struct mage
    {
        void attack() { std::cout << "spell magic curse\n"; }
    };

    void fight_knight(void* k)
    {
        reinterpret_cast<knight*>(k)->attack();
    }

    void fight_mage(void* m)
    {
        reinterpret_cast<mage*>(m)->attack();
    }

    using fight_fn = void(*)(void*);

    void fight(std::vector<std::pair<void*, fight_fn>> const& units)
    {
        for (auto& u : units)
        {
            u.second(u.first);
        }
    }
} // namespace n712e

auto main() -> int {
    {
        using namespace n712e;
        
        knight k;
        mage m;
        
        std::vector<std::pair<void*, fight_fn>> units{
           {&k, &fight_knight},
           {&m, &fight_mage},
        };
        
        fight(units);
    }
    return 0;
}
