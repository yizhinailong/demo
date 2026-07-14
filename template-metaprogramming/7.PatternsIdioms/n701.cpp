import std;

namespace n701 {
    struct game_unit
    {
        virtual void attack() = 0;
    };

    struct knight : game_unit
    {
        void attack() override
        {
            std::cout << "draw sword\n";
        }
    };

    struct mage : game_unit
    {
        void attack() override
        {
            std::cout << "spell magic curse\n";
        }
    };

    void fight(std::vector<game_unit*> const & units)
    {
        for (auto unit : units)
        {
            unit->attack();
        }
    }

    struct knight_mage : game_unit
    {
        void attack() override
        {
            std::cout << "draw magic sword\n";
        }
    };

    struct attack 
    { 
        int value; 
    };

    struct defence
    {
        int value;
    };

    attack operator+(attack const& a, int value)
    {
        return attack{ a.value + value };
    }

    knight_mage operator+(knight const& k, mage const& m)
    {
        return knight_mage{};
    }

    void increment(attack& a) { a.value++; }
    void increment(defence& d) { d.value++; }

    //template <typename T>
    //void increment(T& t) { t.value++; }
} // namespace n701

auto main() -> int {
    {
        using namespace n701;
        
        knight k;
        mage m;
        fight({&k, &m});
        
        knight_mage km = k + m;
        km.attack();
        
        attack a{ 42 };
        a = a + 2;
        
        defence d{ 50 };
        increment(a);
        increment(d);
    }
    return 0;
}
