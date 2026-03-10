#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string_view>
#include <vector>

namespace utils {
    class ScopeProfiler {
    public:
        using ClockType = std::chrono::high_resolution_clock;

        struct Record {
            const char* tag;
            int us;
        };

        explicit ScopeProfiler(const char* tag)
            : ScopeProfiler(tag, ClockType::now()) {
        }

        ~ScopeProfiler() {
            OnDestroy(ClockType::now());
        }

        static std::vector<Record> const& GetRecords() {
            return m_records;
        }

        inline static auto PrintLog(std::ostream& out = std::cout) -> void;

    private:
        inline thread_local static std::vector<Record> m_records;

        ClockType::time_point m_begin;
        ClockType::time_point m_end;
        const char* m_tag;

        inline ScopeProfiler(const char* tag, ClockType::time_point begin);
        inline auto OnDestroy(ClockType::time_point end) -> void;
    };

    ScopeProfiler::ScopeProfiler(const char* tag, ScopeProfiler::ClockType::time_point beg)
        : m_begin(beg),
          m_tag(tag) {
    }

    auto ScopeProfiler::OnDestroy(ScopeProfiler::ClockType::time_point end) -> void {
        auto diff = end - m_begin;
        int us = static_cast<int>(std::chrono::duration_cast<std::chrono::microseconds>(diff).count());
        m_records.push_back({ .tag = m_tag, .us = us });
    }

    auto ScopeProfiler::PrintLog(std::ostream& out) -> void {
        if (m_records.size() == 0) {
            return;
        }

        struct Statistic {
            int max_us = 0;
            int min_us = 0;
            int total_us = 0;
            int count_rec = 0;
            const char* tag = nullptr;
        };

        std::map<std::string_view, Statistic> stats;
        for (auto const& [tag, us] : m_records) {
            auto& stat = stats[tag];
            stat.total_us += us;
            stat.max_us = std::max(stat.max_us, us);
            stat.min_us = (stat.count_rec == 0) ? us : std::min(stat.min_us, us);
            stat.count_rec++;
            stat.tag = tag;
        }

        struct StatisticCompare {
            using value_type = std::pair<std::string_view, Statistic>;

            auto operator()(value_type const& lhs, value_type const& rhs) const -> bool {
                return lhs.second.total_us > rhs.second.total_us;
            }
        };

        std::multiset<std::pair<std::string_view, Statistic>, StatisticCompare> sortstats(stats.begin(), stats.end());

        auto dump = [&out](int val, int w) {
            auto tpwv = 1;
            for (int i = 0; i < w - 1; i++) {
                tpwv *= 10;
            }
            if (val > tpwv) {
                if (val / 1000 > tpwv / 10) {
                    out << std::setw(w - 1) << val / 1000000 << 'M';
                } else {
                    out << std::setw(w - 1) << val / 1000 << 'k';
                }
            } else {
                out << std::setw(w) << val;
            }
        };

        out << "   avg   |   min   |   max   |  total  | cnt | tag\n";
        for (auto const& [tag, stat] : sortstats) {
            dump(stat.total_us / stat.count_rec, 9);
            out << '|';
            dump(stat.min_us, 9);
            out << '|';
            dump(stat.max_us, 9);
            out << '|';
            dump(stat.total_us, 9);
            out << '|';
            dump(stat.count_rec, 5);
            out << '|';
            out << ' ' << tag << '\n';
        }
    }

#if defined(__GUNC__) || defined(__clang__)
    #define DefScopeProfiler ScopeProfiler _scopeProfiler(__PRETTY_FUNCTION__);
#elif defined(_MSC_VER)
    #define DefScopeProfiler ScopeProfiler _scopeProfiler(__FUNCSIG__);
#else
    #define DefScopeProfiler ScopeProfiler _scopeProfiler(__func__);
#endif

    template <class T>
    static
#if defined(__GUNC__) || defined(__clang__)
        __attribute__((noinline))
#elif defined(_MSC_VER)
        __declspec(noinline)
#endif
        void doNotOptimize(T volatile const& t) {
    }
} // namespace utils
