import std;

namespace n406 {
    template <typename T>
    struct base_parser {
        template <typename U>
        void init() {
            std::cout << "init\n";
        }

        template <typename U>
        struct token {};
    };

    template <typename T>
    struct parser : base_parser<T> {
        void parse() {
            // base_parser<T>::init<int>(); // error

            base_parser<T>::template init<int>();

            using token_type = base_parser<T>::template token<int>;
            token_type t1{};

            typename base_parser<T>::template token<int> t2{};

            std::cout << "parse\n";
        }
    };
} // namespace n406

auto main() -> int {
    {
        using namespace n406;

        parser<int> p;
        p.parse();
    }
    return 0;
}
