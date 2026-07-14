import std;

namespace n712d {
    struct knight
    {
        void attack() { std::cout << "draw sword\n"; }
    };

    struct mage
    {
        void attack() { std::cout << "spell magic curse\n"; }
    };

    struct unit
    {
        template <typename T>
        unit(T&& obj) : unit_(std::make_shared<unit_model<T>>(std::forward<T>(obj))) {}

        void attack()
        {
            unit_->attack();
        }

        struct unit_concept
        {
            virtual void attack() = 0;
            virtual ~unit_concept() = default;
        };

        template <typename T>
        struct unit_model : public unit_concept
        {
            unit_model(T& unit) : t(unit) {}

            void attack() override { t.attack(); }
        private:
            T& t;
        };

    private:
        std::shared_ptr<unit_concept> unit_;
    };

    void fight(std::vector<unit>& units)
    {
        for (auto& u : units)
            u.attack();
    }
} // namespace n712d

auto main() -> int {
    {
        using namespace n712d;
        
        knight k;
        mage m;
        
        std::vector<unit> v{ unit(k), unit(m) };
        
        fight(v);
    }

    {
        using namespace n712d;
        
        std::any u;
        
        u = knight{};
        if (u.has_value())
           std::any_cast<knight>(u).attack();
        
        u = mage{};
        if (u.has_value())
           std::any_cast<mage>(u).attack();
    }
    return 0;
}
