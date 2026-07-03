import std;

namespace n405 {
    template <typename T>
    struct base_parser {
        using value_type = T;
    };

    template <typename T>
    struct parser : base_parser<T> {
        void parse() {
            // value_type v{}; // syntax error: unexpected token 'identifier', expected ';'
            // base_parser<T>::value_type v{};
            typename base_parser<T>::value_type v{};

            std::cout << "parse\n";
        }
    };
} // namespace n405

auto main() -> int {
    {
        using namespace n405;

        parser<int> p;
        p.parse();
    }
    return 0;
}
