import std;

namespace n705 {
    template <typename T>
    struct movable_unit
    {
        void advance(size_t steps)
        {
            while (steps--)
                static_cast<T*>(this)->step_forth();
        }

        void retreat(size_t steps)
        {
            while (steps--)
                static_cast<T*>(this)->step_back();
        }
    };

    struct knight : movable_unit<knight>
    {
        void step_forth() 
        {
            std::cout << "knight moves forward\n";
        }

        void step_back()
        {
            std::cout << "knight moves back\n";
        }
    };

    struct mage : movable_unit<mage>
    {
        void step_forth()
        {
            std::cout << "mage moves forward\n";
        }

        void step_back()
        {
            std::cout << "mage moves back\n";
        }
    };
} // namespace n705

auto main() -> int {
    {
        using namespace n705;
        
        knight k;
        k.advance(3);
        k.retreat(2);
        
        mage m;
        m.advance(5);
        m.retreat(3);
    }
    return 0;
}
