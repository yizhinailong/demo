import std;

namespace n709b {
    struct building : std::enable_shared_from_this<building>
    {
    };
} // namespace n709b

auto main() -> int {
    {
        using namespace n709b;
        
        building* b = new building();
        
        std::shared_ptr<building> p1{ b };
        std::shared_ptr<building> p2{ b->shared_from_this()}; // OK
    }
    return 0;
}
