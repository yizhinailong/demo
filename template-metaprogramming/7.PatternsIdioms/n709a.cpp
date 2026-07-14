import std;

namespace n709a {
    struct building {};
} // namespace n709a

auto main() -> int {
    {
        using namespace n709a;
        
        building* b = new building();
        
        std::shared_ptr<building> p1{ b };
        //std::shared_ptr<building> p2{ b }; // bad
    }
    return 0;
}
