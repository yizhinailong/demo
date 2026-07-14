import std;

namespace n612 {
    template<typename T>
    concept arithmetic = requires 
    {
        std::is_arithmetic_v<T>; 
    };

    template <typename T>
    concept addable = requires(T a, T b) 
    { 
        a + b; 
    };

    template <typename T>
    concept logger = requires(T t)
    {
        t.error("just");
        t.warning("a");
        t.info("demo");
    };

    template <logger T>
    void log_error(T& logger)
    {
    }

    struct console_logger
    {
        void error(std::string_view text){}
        void warning(std::string_view text) {}
        void info(std::string_view text) {}
    };

    struct stream_logger
    {
        void error(std::string_view text, bool = false) {}
        void warning(std::string_view text, bool = false) {}
        void info(std::string_view text, bool) {}
    };
} // namespace n612

auto main() -> int {
    {
        using namespace n612;
        
        console_logger cl;
        log_error(cl);
        
        [[maybe_unused]] stream_logger sl;
        // log_error(sl); // error: the associated constraints are not satisfied
    }
    return 0;
}
