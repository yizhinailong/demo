import std;

namespace n224 {
    template <typename T>
    struct control {};

    template <typename T>
    struct button : public control<T> {};

    auto show(button<int>* ptr) -> void {
        control<int>* c = ptr;
    }
} // namespace n224

auto main() -> int {
    return 0;
}
