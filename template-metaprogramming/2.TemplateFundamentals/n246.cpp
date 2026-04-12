import std;

namespace n246 {
    template <typename T>
    struct foo {
        using value_type = T;
    };
} // namespace n246

auto main() -> int {
    {
        using index_t       = int;
        using NameValueList = std::vector<std::pair<int, std::string>>;
        using fn_ptr        = int(int, char);
    }
    return 0;
}
