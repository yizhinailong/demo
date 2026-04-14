import std;

namespace n318 {
    template <typename... T>
    struct outer {
        template <T... args>
        struct inner {};
    };

    template <typename... T>
    struct tag {
    };

    template <typename T, typename U, typename... Args>
    void tagger() {
        [[maybe_unused]] tag<T, U, Args...> t1;
        [[maybe_unused]] tag<T, Args..., U> t2;
        [[maybe_unused]] tag<Args..., T, U> t3;
        [[maybe_unused]] tag<U, T, Args...> t4;
    }

    template <typename... Args>
    void make_it(Args... args) {
    }

    template <typename T>
    T step_it(T value) {
        return value + 1;
    }

    template <typename... T>
    int sum(T... args) {
        return (... + args);
    }

    template <typename... T>
    void do_sums(T... args) {
        auto s1 = sum(args...);
        auto s2 = sum(42, args...);
        auto s3 = sum(step_it(args)...);
    }

    template <typename... T>
    struct sum_wrapper {
        sum_wrapper(T... args) {
            value = (... + args);
        }

        std::common_type_t<T...> value;
    };

    template <typename... T>
    void parenthesized(T... args) {
        std::array<std::common_type_t<T...>, sizeof...(T)> arr{ args... };

        sum_wrapper sw1(args...);   // value = 1 + 2 + 3 + 4
        sum_wrapper sw2(++args...); // value = 2 + 3 + 4 + 5
    }

    template <typename... T>
    void brace_enclosed(T... args) {
        int arr1[sizeof...(args) + 1] = { args..., 0 };       // 1,2,3,4,5
        int arr2[sizeof...(args)]     = { step_it(args)... }; // 2,3,4,5
    }

    struct A {
        void execute() { std::cout << "A::execute\n"; }
    };

    struct B {
        void execute() { std::cout << "B::execute\n"; }
    };

    struct C {
        void execute() { std::cout << "C::execute\n"; }
    };

    template <typename... Bases>
    struct X : public Bases... {
        X(Bases const&... args) : Bases(args)... {
        }

        using Bases::execute...;
    };

    template <typename... T>
    void captures(T... args) {
        auto l = [args...] {
            return sum(step_it(args)...);
        };
        auto s = l();
    }

    template <typename... T>
    auto make_array(T... args) {
        return std::array<std::common_type_t<T...>, sizeof...(T)>{ args... };
    };

    template <typename... T>
    struct alignment1 {
        alignas(T...) char a;
    };

    template <int... args>
    struct alignment2 {
        alignas(args...) char a;
    };
} // namespace n318

auto main() -> int {
    {
        using namespace n318;

        [[maybe_unused]] outer<int, double, char[5]> a;

        tagger<int, double, char[5], short, float>();

        make_it(42);
        make_it(42, 'a');

        do_sums(1, 2, 3, 4);

        parenthesized(1, 2, 3, 4);

        brace_enclosed(1, 2, 3, 4);

        captures(1, 2, 3, 4);

        auto arr = make_array(1, 2, 3, 4);

        alignment1<int, double> al1;
        al1.a = 'a';

        // alignment2<1, 4, 8> al2; // error with VC++
        // al2.a = 'b';
    }

    {
        using namespace n318;

        A a;
        B b;
        C c;
        X x(a, b, c);

        x.A::execute();
        x.B::execute();
        x.C::execute();
    }
    return 0;
}
