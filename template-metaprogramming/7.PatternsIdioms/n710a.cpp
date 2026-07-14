import std;

namespace n710a {
    struct hit_and_run
    {
        void fight()
        {
            std::cout << "hit once hard then run\n";
        }
    };

    struct last_man_standing
    {
        void fight()
        {
            std::cout << "duel until one falls\n";
        }
    };

    template <typename Strategy>
    struct knight : public Strategy
    {
        void attack()
        {
            std::cout << "draw sword\n";
            Strategy::fight();
        }
    };

    template <typename Strategy>
    struct mage : public Strategy
    {
        void attack()
        {
            std::cout << "spell magic curse\n";
            Strategy::fight();
        }
    };
} // namespace n710a

auto main() -> int {
    {
        using namespace n710a;
        
        knight<last_man_standing> k;
        mage<hit_and_run> m;
        
        k.attack();
        m.attack();
    }
    return 0;
}
