import std;

namespace n403 {
    template <typename T>
    struct base_parser {
        void init() {
            std::cout << "init\n";
        }
    };

    template <typename T>
    struct parser : base_parser<T> {
        void parse() {
            // init();        // error: identifier not found
            this->init();

            std::cout << "parse\n";
        }
    };
} // namespace n403

auto main() -> int {
    {
        using namespace n403;

        parser<int> p;
        p.parse();
    }
    return 0;
}
