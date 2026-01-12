#pragma once

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace cmdline {
    namespace detail {
        template <typename Target, typename Source, bool Same>
        class lexical_cast_t {
        public:
            static auto cast(const Source& arg) -> Target {
                Target ret;
                std::stringstream ss;
                if (!(ss << arg && ss >> ret && ss.eof())) {
                    throw std::bad_cast();
                }

                return ret;
            }
        };

        template <typename Target, typename Source>
        class lexical_cast_t<Target, Source, true> {
        public:
            static auto cast(const Source& arg) -> Target {
                return arg;
            }
        };

        template <typename Source>
        class lexical_cast_t<std::string, Source, false> {
        public:
            static auto cast(const Source& arg) -> std::string {
                std::ostringstream ss;
                ss << arg;
                return ss.str();
            }
        };

        template <typename Target>
        class lexical_cast_t<Target, std::string, false> {
        public:
            static auto cast(const std::string& arg) -> Target {
                Target ret;
                std::istringstream ss(arg);
                if (!(ss >> ret && ss.eof())) {
                    throw std::bad_cast();
                }
                return ret;
            }
        };

        template <typename Target, typename Source>
        auto lexical_cast(const Source& arg) -> Target {
            return lexical_cast_t<Target, Source, std::is_same<Target, Source>::value>::cast(arg);
        }

        template <typename T>
        constexpr auto type_name_impl() -> std::string_view {
#if defined(__clang__)
            constexpr std::string_view prefix = "[T = ";
            constexpr std::string_view suffix = "]";
            constexpr std::string_view func = __PRETTY_FUNCTION__;
#elif defined(__GNUC__)
            constexpr std::string_view prefix = "with T = ";
            constexpr std::string_view suffix = "]";
            constexpr std::string_view func = __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
            constexpr std::string_view prefix = "type_name_impl<";
            constexpr std::string_view suffix = ">(void)";
            constexpr std::string_view func = __FUNCSIG__;
#else
            return "unknown";
#endif
            auto start = func.find(prefix) + prefix.size();
            auto end = func.rfind(suffix);
            return func.substr(start, end - start);
        }

        template <typename T>
        auto readable_typename() -> std::string {
            return std::string(type_name_impl<T>());
        }

        template <typename T>
        auto default_value(T def) -> std::string {
            return detail::lexical_cast<std::string>(def);
        }

        template <>
        inline auto readable_typename<std::string>() -> std::string {
            return "string";
        }

    } // namespace detail

    //-----

    class cmdline_error : public std::exception {
    public:
        explicit cmdline_error(std::string msg)
            : m_msg(std::move(msg)) {}

        ~cmdline_error() noexcept = default;
        cmdline_error(const cmdline_error&) = default;
        cmdline_error& operator=(const cmdline_error&) = default;
        cmdline_error(cmdline_error&&) noexcept = default;
        cmdline_error& operator=(cmdline_error&&) noexcept = default;

        [[nodiscard]]
        auto what() const noexcept -> const char* override {
            return m_msg.c_str();
        }

    private:
        std::string m_msg;
    };

    template <typename T>
    struct default_reader {
        auto operator()(const std::string& str) -> T {
            return detail::lexical_cast<T>(str);
        }
    };

    template <typename T>
    struct range_reader {
        range_reader(const T& low, const T& high)
            : low(low), high(high) {}

        auto operator()(const std::string& s) const -> T {
            T ret = default_reader<T>()(s);
            if (!(ret >= low && ret <= high)) {
                throw cmdline::cmdline_error("range_error");
            }
            return ret;
        }

    private:
        T low, high;
    };

    template <typename T>
    auto range(const T& low, const T& high) -> range_reader<T> {
        return range_reader<T>(low, high);
    }

    template <typename T>
    struct oneof_reader {
        auto operator()(const std::string& s) -> T {
            T ret = default_reader<T>()(s);
            if (std::find(alt.begin(), alt.end(), ret) == alt.end()) {
                throw cmdline_error("");
            }
            return ret;
        }

        auto add(const T& v) -> void {
            alt.push_back(v);
        }

    private:
        std::vector<T> alt;
    };

    template <typename T, typename... Args>
    auto oneof(T arg, Args... args) -> oneof_reader<T> {
        oneof_reader<T> ret;
        ret.add(arg);
        (ret.add(args), ...);
        return ret;
    }

    //-----

    class parser {
    public:
        parser() = default;

        ~parser() {
            for (auto& m_option : m_options) {
                delete m_option.second;
            }
        }

        auto add(const std::string& name,
                 char short_name = 0,
                 const std::string& desc = "") -> void {
            if (m_options.count(name) != 0U) {
                throw cmdline_error("multiple definition: " + name);
            }
            m_options[name] = new option_without_value(name, short_name, desc);
            m_ordered.push_back(m_options[name]);
        }

        template <typename T>
        auto add(const std::string& name,
                 char short_name = 0,
                 const std::string& desc = "",
                 bool need = true,
                 const T def = T()) -> void {
            add(name, short_name, desc, need, def, default_reader<T>());
        }

        template <typename T, typename F>
        auto add(const std::string& name,
                 char short_name = 0,
                 const std::string& desc = "",
                 bool need = true,
                 const T def = T(),
                 F reader = F()) -> void {
            if (m_options.count(name)) {
                throw cmdline_error("multiple definition: " + name);
            }
            m_options[name] = new option_with_value_with_reader<T, F>(name, short_name, need, def, desc, reader);
            m_ordered.push_back(m_options[name]);
        }

        auto footer(const std::string& foot) -> void {
            m_ftr = foot;
        }

        auto set_program_name(const std::string& name) -> void {
            m_prog_name = name;
        }

        auto exist(const std::string& name) const -> bool {
            if (m_options.count(name) == 0) {
                throw cmdline_error("there is no flag: --" + name);
            }
            return m_options.find(name)->second->has_set();
        }

        template <typename T>
        auto get(const std::string& name) const -> const T& {
            if (m_options.count(name) == 0) {
                throw cmdline_error("there is no flag: --" + name);
            }
            const option_with_value<T>* p = dynamic_cast<const option_with_value<T>*>(m_options.find(name)->second);
            if (p == NULL) {
                throw cmdline_error("type mismatch flag '" + name + "'");
            }
            return p->get();
        }

        auto rest() const -> const std::vector<std::string>& {
            return m_others;
        }

        auto parse(const std::string& arg) -> bool {
            std::vector<std::string> args;

            std::string buf;
            bool in_quote = false;
            for (std::string::size_type i = 0; i < arg.length(); i++) {
                if (arg[i] == '\"') {
                    in_quote = !in_quote;
                    continue;
                }

                if (arg[i] == ' ' && !in_quote) {
                    args.push_back(buf);
                    buf = "";
                    continue;
                }

                if (arg[i] == '\\') {
                    i++;
                    if (i >= arg.length()) {
                        m_errors.emplace_back("unexpected occurrence of '\\' at end of string");
                        return false;
                    }
                }

                buf += arg[i];
            }

            if (in_quote) {
                m_errors.push_back("quote is not closed");
                return false;
            }

            if (buf.length() > 0) {
                args.push_back(buf);
            }

            for (size_t i = 0; i < args.size(); i++) {
                std::cout << "\"" << args[i] << "\"" << std::endl;
            }

            return parse(args);
        }

        auto parse(const std::vector<std::string>& args) -> bool {
            int argc = static_cast<int>(args.size());
            std::vector<const char*> argv(argc);

            for (int i = 0; i < argc; i++) {
                argv[i] = args[i].c_str();
            }

            return parse(argc, &argv[0]);
        }

        auto parse(int argc, const char* const argv[]) -> bool {
            m_errors.clear();
            m_others.clear();

            if (argc < 1) {
                m_errors.push_back("argument number must be longer than 0");
                return false;
            }
            if (m_prog_name == "") {
                m_prog_name = argv[0];
            }

            std::map<char, std::string> lookup;
            for (std::map<std::string, option_base*>::iterator p = m_options.begin();
                 p != m_options.end();
                 p++) {
                if (p->first.length() == 0) {
                    continue;
                }
                char initial = p->second->short_name();
                if (initial) {
                    if (lookup.count(initial) > 0) {
                        lookup[initial] = "";
                        m_errors.push_back(std::string("short option '") + initial + "' is ambiguous");
                        return false;
                    } else {
                        lookup[initial] = p->first;
                    }
                }
            }

            for (int i = 1; i < argc; i++) {
                if (strncmp(argv[i], "--", 2) == 0) {
                    const char* p = strchr(argv[i] + 2, '=');
                    if (p) {
                        std::string name(argv[i] + 2, p);
                        std::string val(p + 1);
                        set_option(name, val);
                    } else {
                        std::string name(argv[i] + 2);
                        if (m_options.count(name) == 0) {
                            m_errors.push_back("undefined option: --" + name);
                            continue;
                        }
                        if (m_options[name]->has_value()) {
                            if (i + 1 >= argc) {
                                m_errors.push_back("option needs value: --" + name);
                                continue;
                            } else {
                                i++;
                                set_option(name, argv[i]);
                            }
                        } else {
                            set_option(name);
                        }
                    }
                } else if (strncmp(argv[i], "-", 1) == 0) {
                    if (!argv[i][1]) {
                        continue;
                    }
                    char last = argv[i][1];
                    for (int j = 2; argv[i][j]; j++) {
                        last = argv[i][j];
                        if (lookup.count(argv[i][j - 1]) == 0) {
                            m_errors.push_back(std::string("undefined short option: -") + argv[i][j - 1]);
                            continue;
                        }
                        if (lookup[argv[i][j - 1]] == "") {
                            m_errors.push_back(std::string("ambiguous short option: -") + argv[i][j - 1]);
                            continue;
                        }
                        set_option(lookup[argv[i][j - 1]]);
                    }

                    if (lookup.count(last) == 0) {
                        m_errors.push_back(std::string("undefined short option: -") + last);
                        continue;
                    }
                    if (lookup[last] == "") {
                        m_errors.push_back(std::string("ambiguous short option: -") + last);
                        continue;
                    }

                    if (i + 1 < argc && m_options[lookup[last]]->has_value()) {
                        set_option(lookup[last], argv[i + 1]);
                        i++;
                    } else {
                        set_option(lookup[last]);
                    }
                } else {
                    m_others.push_back(argv[i]);
                }
            }

            for (auto& m_option : m_options) {
                if (!m_option.second->valid()) {
                    m_errors.push_back("need option: --" + std::string(m_option.first));
                }
            }

            return m_errors.size() == 0;
        }

        auto parse_check(const std::string& arg) -> void {
            if (!m_options.count("help")) {
                add("help", '?', "print this message");
            }
            check(0, parse(arg));
        }

        auto parse_check(const std::vector<std::string>& args) -> void {
            if (!m_options.count("help")) {
                add("help", '?', "print this message");
            }
            check(args.size(), parse(args));
        }

        auto parse_check(int argc, char* argv[]) -> void {
            if (!m_options.count("help")) {
                add("help", '?', "print this message");
            }
            check(argc, parse(argc, argv));
        }

        auto error() const -> std::string {
            return m_errors.size() > 0 ? m_errors[0] : "";
        }

        auto error_full() const -> std::string {
            std::ostringstream oss;
            for (size_t i = 0; i < m_errors.size(); i++) {
                oss << m_errors[i] << std::endl;
            }
            return oss.str();
        }

        auto usage() const -> std::string {
            std::ostringstream oss;
            oss << "usage: " << m_prog_name << " ";
            for (size_t i = 0; i < m_ordered.size(); i++) {
                if (m_ordered[i]->must()) {
                    oss << m_ordered[i]->short_description() << " ";
                }
            }

            oss << "[options] ... " << m_ftr << std::endl;
            oss << "options:" << std::endl;

            size_t max_width = 0;
            for (size_t i = 0; i < m_ordered.size(); i++) {
                max_width = std::max(max_width, m_ordered[i]->name().length());
            }
            for (size_t i = 0; i < m_ordered.size(); i++) {
                if (m_ordered[i]->short_name()) {
                    oss << "  -" << m_ordered[i]->short_name() << ", ";
                } else {
                    oss << "      ";
                }

                oss << "--" << m_ordered[i]->name();
                for (size_t j = m_ordered[i]->name().length(); j < max_width + 4; j++) {
                    oss << ' ';
                }
                oss << m_ordered[i]->description() << std::endl;
            }
            return oss.str();
        }

    private:
        auto check(int argc, bool ok) -> void {
            if ((argc == 1 && !ok) || exist("help")) {
                std::cerr << usage();
                exit(0);
            }

            if (!ok) {
                std::cerr << error() << std::endl
                          << usage();
                exit(1);
            }
        }

        auto set_option(const std::string& name) -> void {
            if (m_options.count(name) == 0) {
                m_errors.push_back("undefined option: --" + name);
                return;
            }
            if (!m_options[name]->set()) {
                m_errors.push_back("option needs value: --" + name);
                return;
            }
        }

        auto set_option(const std::string& name, const std::string& value) -> void {
            if (m_options.count(name) == 0) {
                m_errors.push_back("undefined option: --" + name);
                return;
            }
            if (!m_options[name]->set(value)) {
                m_errors.push_back("option value is invalid: --" + name + "=" + value);
                return;
            }
        }

        class option_base {
        public:
            virtual ~option_base() = default;

            virtual auto has_value() const -> bool = 0;
            virtual auto set() -> bool = 0;
            virtual auto set(const std::string& value) -> bool = 0;
            virtual auto has_set() const -> bool = 0;
            virtual auto valid() const -> bool = 0;
            virtual auto must() const -> bool = 0;

            virtual auto name() const -> const std::string& = 0;
            virtual auto short_name() const -> char = 0;
            virtual auto description() const -> const std::string& = 0;
            virtual auto short_description() const -> std::string = 0;
        };

        class option_without_value : public option_base {
        public:
            option_without_value(const std::string& name,
                                 char short_name,
                                 const std::string& desc)
                : m_nam(name), m_snam(short_name), m_desc(desc), m_has(false) {
            }

            ~option_without_value() override = default;

            auto has_value() const -> bool override {
                return false;
            }

            auto set() -> bool override {
                m_has = true;
                return true;
            }

            auto set(const std::string&) -> bool override {
                return false;
            }

            auto has_set() const -> bool override {
                return m_has;
            }

            auto valid() const -> bool override {
                return true;
            }

            auto must() const -> bool override {
                return false;
            }

            auto name() const -> const std::string& override {
                return m_nam;
            }

            auto short_name() const -> char override {
                return m_snam;
            }

            auto description() const -> const std::string& override {
                return m_desc;
            }

            auto short_description() const -> std::string override {
                return "--" + m_nam;
            }

        private:
            std::string m_nam;
            char m_snam;
            std::string m_desc;
            bool m_has;
        };

        template <typename T>
        class option_with_value : public option_base {
        public:
            option_with_value(const std::string& name,
                              char short_name,
                              bool need,
                              const T& def,
                              const std::string& desc)
                : m_nam(name), m_snam(short_name), m_need(need), m_has(false), m_def(def), m_actual(def) {
                this->m_desc = full_description(desc);
            }

            ~option_with_value() override = default;

            auto get() const -> const T& {
                return m_actual;
            }

            auto has_value() const -> bool override {
                return true;
            }

            auto set() -> bool override {
                return false;
            }

            auto set(const std::string& value) -> bool override {
                try {
                    m_actual = read(value);
                    m_has = true;
                } catch (const std::exception& e) {
                    return false;
                }
                return true;
            }

            auto has_set() const -> bool override {
                return m_has;
            }

            auto valid() const -> bool override {
                if (m_need && !m_has) {
                    return false;
                }
                return true;
            }

            auto must() const -> bool override {
                return m_need;
            }

            auto name() const -> const std::string& override {
                return m_nam;
            }

            auto short_name() const -> char override {
                return m_snam;
            }

            auto description() const -> const std::string& override {
                return m_desc;
            }

            auto short_description() const -> std::string override {
                return "--" + m_nam + "=" + detail::readable_typename<T>();
            }

        protected:
            auto full_description(const std::string& desc) -> std::string {
                return desc + " (" + detail::readable_typename<T>() +
                       (m_need ? "" : " [=" + detail::default_value<T>(m_def) + "]") + ")";
            }

            virtual auto read(const std::string& s) -> T = 0;

        private:
            std::string m_nam;
            char m_snam;
            bool m_need;
            std::string m_desc;

            bool m_has;
            T m_def;
            T m_actual;
        };

        template <typename T, typename F>
        class option_with_value_with_reader : public option_with_value<T> {
        public:
            option_with_value_with_reader(
                const std::string& name,
                char short_name,
                bool need,
                const T def,
                const std::string& desc,
                F reader)
                : option_with_value<T>(name, short_name, need, def, desc), m_reader(reader) {
            }

        private:
            auto read(const std::string& s) -> T override {
                return m_reader(s);
            }

            F m_reader;
        };

        std::map<std::string, option_base*> m_options;
        std::vector<option_base*> m_ordered;
        std::string m_ftr;

        std::string m_prog_name;
        std::vector<std::string> m_others;

        std::vector<std::string> m_errors;
    };
} // namespace cmdline
