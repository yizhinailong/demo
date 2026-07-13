import std;

namespace n412 {
    template <typename T>
    void process(T arg) {
        std::cout << "processing " << arg << '\n';
    }

    struct account_t {
        int number;

        int get_number() { return number; }

        int from_string(std::string text) { return std::atoi(text.c_str()); }
    };

    struct transaction_t {
        double amount;
    };

    struct balance_report_t {};

    struct balance_t {
        account_t account;
        double    amount;

        account_t get_account() { return account; }

        int get_account_number() { return account.number; }

        bool can_withdraw(double const value) { return amount >= value; };

        transaction_t withdraw(double const value) {
            amount -= value;
            return transaction_t{ -value };
        }

        balance_report_t make_report(int const type) { return {}; }
    };

    template <typename T>
    void process01(T) {
        std::cout << "T" << '\n';
    }

    template <typename T>
    void process02(T const) {
        std::cout << "T const" << '\n';
    }

    template <typename T>
    void process03(T volatile) {
        std::cout << "T volatile" << '\n';
    }

    template <typename T>
    void process04(T*) {
        std::cout << "T*" << '\n';
    }

    template <typename T>
    void process04(T&) {
        std::cout << "T&" << '\n';
    }

    template <typename T>
    void process05(T&&) {
        std::cout << "T&&" << '\n';
    }

    template <typename T>
    void process06(T[5]) {
        std::cout << "T[5]" << '\n';
    }

    template <size_t n>
    void process07(account_t[5][n]) {
        std::cout << "C[5][n]" << '\n';
    }

    template <typename T>
    void process08(T (*)()) {
        std::cout << "T (*)()" << '\n';
    }

    template <typename T>
    void process08(account_t (*)(T)) {
        std::cout << "C (*)(T)" << '\n';
    }

    template <typename T, typename U>
    void process08(T (*)(U)) {
        std::cout << "T (*)(U)" << '\n';
    }

    template <typename T>
    void process09(T (account_t::*)()) {
        std::cout << "T (C::*)()" << '\n';
    }

    template <typename T, typename U>
    void process09(T (account_t::*)(U)) {
        std::cout << "T (C::*)(U)" << '\n';
    }

    template <typename T, typename U>
    void process09(T (U::*)()) {
        std::cout << "T (U::*)()" << '\n';
    }

    template <typename T, typename U, typename V>
    void process09(T (U::*)(V)) {
        std::cout << "T (U::*)(V)" << '\n';
    }

    template <typename T>
    void process09(account_t (T::*)()) {
        std::cout << "C (T::*)()" << '\n';
    }

    template <typename T, typename U>
    void process09(transaction_t (T::*)(U)) {
        std::cout << "C (T::*)(U)" << '\n';
    }

    template <typename T>
    void process09(balance_report_t (balance_t::*)(T)) {
        std::cout << "D (C::*)(T)" << '\n';
    }

    template <typename T>
    void process10(T account_t::*) {
        std::cout << "T C::*" << '\n';
    }

    template <typename T>
    void process10(account_t T::*) {
        std::cout << "C T::*" << '\n';
    }

    template <typename T, typename U>
    void process10(T U::*) {
        std::cout << "T U::*" << '\n';
    }

    template <typename T>
    struct wrapper {
        T data;
    };

    template <size_t i>
    struct int_array {
        int data[i];
    };

    template <typename T>
    void process11(wrapper<T>) {
        std::cout << "C<T>" << '\n';
    }

    template <size_t i>
    void process12(int_array<i>) {
        std::cout << "C<i>" << '\n';
    }

    template <template <typename> class TT, typename T>
    void process13(TT<T>) {
        std::cout << "TT<T>" << '\n';
    }

    template <template <size_t> typename TT, size_t i>
    void process14(TT<i>) {
        std::cout << "TT<i>" << '\n';
    }

    template <template <typename> typename TT>
    void process15(TT<account_t>) {
        std::cout << "TT<C>" << '\n';
    }
} // namespace n412

auto main() -> int {
    {
        using namespace n412;

        process(42);        // T is int
        process<int>(42);   // T is int, redundant
        process<short>(42); // T is short
    }

    {
        using namespace n412;

        account_t ac{ 42 };
        process01(ac);  // T
        process02(ac);  // T const
        process03(ac);  // T volatile

        process04(&ac); // T*
        process04(ac);  // T&
        process05(ac);  // T&&

        account_t arr1[5]{};
        process06(arr1); // T[5]

        process06(&ac);  // T[5]

        account_t arr2[5][3];
        process07(arr2); // C[5][n]

        account_t (*pf1)()    = nullptr;
        account_t (*pf2)(int) = nullptr;
        double (*pf3)(int)    = nullptr;
        process08(pf1); // T (*)()
        process08(pf2); // C (*)(T)
        process08(pf3); // T (*)(U)

        int (account_t::*pfm1)()                 = &account_t::get_number;
        int (account_t::*pfm2)(std::string)      = &account_t::from_string;
        int (balance_t::*pfm3)()                 = &balance_t::get_account_number;
        bool (balance_t::*pfm4)(double)          = &balance_t::can_withdraw;
        account_t (balance_t::*pfm5)()           = &balance_t::get_account;
        transaction_t (balance_t::*pfm6)(double) = &balance_t::withdraw;
        balance_report_t (balance_t::*pfm7)(int) = &balance_t::make_report;

        process09(pfm1); // T (C::*)()
        process09(pfm2); // T (C::*)(U)
        process09(pfm3); // T (U::*)()
        process09(pfm4); // T (U::*)(V)
        process09(pfm5); // C (T::*)()
        process09(pfm6); // C (T::*)(U)
        process09(pfm7); // D (C::*)(T)

        // process10(&account_t::number);   // T C::*
        process10(&balance_t::account); // C T::*
        process10(&balance_t::amount);  // T U::*

        wrapper<double> wd{ 42.0 };
        process11(wd); // C<T>

        int_array<5> ia{};
        process12(ia); // C<i>

        process13(wd); // TT<U>
        process14(ia); // TT<i>

        wrapper<account_t> wa{ { 42 } };
        process15(wa); // TT<C>
    }

    {
        using namespace n412;
        [[maybe_unused]] decltype(wrapper<double>::data) e1; // double

        int                                               a = 42;
        [[maybe_unused]] decltype(wrapper<char>::data, a) e2 = a; // int&
    }
    return 0;
}
