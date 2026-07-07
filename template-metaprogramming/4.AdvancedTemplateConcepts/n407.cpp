import std;

namespace n407 {
    template <typename T>
    struct parser {
        parser*          p1; // parser is the CI
        parser<T>*       p2; // parser<T> is the CI
        n407::parser<T>* p3; // ::parser<T> is the CI

        // parser<T*> p4;     // parser<T*> is not the CI

        struct token {
            token*            t1; // token is the CI
            parser<T>::token* t2; // parser<T>::token is the CI
            // typename parser<T*>::token* t3; // parser<T*>::token is not the CI
        };
    };

    template <typename T>
    struct parser<T*> {
        parser<T*>* p1; // parser<T*> is the CI
        parser<T>*  p2; // parser<T> is not the CI
    };
} // namespace n407

auto main() -> int {
    {
        using namespace n407;

        [[maybe_unused]]
        parser<int> p;
    }
    return 0;
}
