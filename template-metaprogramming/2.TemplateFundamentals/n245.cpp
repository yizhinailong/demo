import std;

namespace n245 {
    template <typename T>
    struct foo {
        typedef T value_type;
    };
} // namespace n245

auto main() -> int {
    {
        typedef int                                      index_t;
        typedef std::vector<std::pair<int, std::string>> NameValueList;
        typedef int (*fn_ptr)(int, char);
    }
    return 0;
}
