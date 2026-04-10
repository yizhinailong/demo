import std;

namespace n210 {
    struct device {
        virtual void output() = 0;
        virtual ~device()     = default;
    };

    template <typename Command, void (Command::*action)()>
    struct smart_device : device {
    public:
        smart_device(Command* cmd)
            : m_cmd(cmd) {}

        void output() override {
            (m_cmd->*action)();
        }

    private:
        Command* m_cmd;
    };

    struct hello_command {
        auto say_hello_english() -> void {
            std::println("Hello World.");
        }

        auto say_hello_chinese() {
            std::println("你好世界。");
        }
    };
} // namespace n210

auto main() -> int {
    {
        using namespace n210;

        hello_command cmd;

        auto w1 = std::make_unique<smart_device<hello_command, &hello_command::say_hello_english>>(&cmd);
        w1->output();

        auto w2 = std::make_unique<smart_device<hello_command, &hello_command::say_hello_chinese>>(&cmd);
        w2->output();
    }
    return 0;
}
