import std;

namespace n212 {
    struct device {
        virtual void output() = 0;
        virtual ~device()     = default;
    };

    template <void (*action)()>
    struct hello_command : device {
        void output() override {
            (*action)();
        }
    };

    auto say_hello_chinese() {
        std::println("你好世界。");
    }

    auto say_hello_english() {
        std::println("Hello World.");
    }

} // namespace n212

auto main() -> int {
    {
        using namespace n212;

        auto w1 = std::make_unique<hello_command<&say_hello_chinese>>();
        w1->output();

        auto w2 = std::make_unique<hello_command<&say_hello_english>>();
        w2->output();
    }

    {
        using namespace n212;

        std::shared_ptr<device> w1 = std::make_unique<hello_command<&say_hello_chinese>>();
        w1->output();

        std::shared_ptr<device> w2 = std::make_unique<hello_command<&say_hello_english>>();
        w2->output();
    }
    return 0;
}
