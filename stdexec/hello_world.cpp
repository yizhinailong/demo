#include <print>
#include <tuple>
#include <utility>

// Pull in the reference implementation of P2300:
#include <stdexec/execution.hpp>

#include "exec/static_thread_pool.hpp"

auto main() -> int {
    exec::static_thread_pool ctx{ 8 };
    stdexec::scheduler auto  sch  = ctx.get_scheduler();

    stdexec::sender auto begin    = stdexec::schedule(sch);
    stdexec::sender auto hi_again = stdexec::then(
        begin,
        [] {
            std::println("Hello world! Have an int.");
            return 13;
        }
    );

    stdexec::sender auto add_42 = stdexec::then(hi_again, [](int arg) { return arg + 42; });
    auto [i]                    = stdexec::sync_wait(std::move(add_42)).value();
    std::println("Result: {}", i);

    // Sync_wait provides a run_loop scheduler
    auto t = stdexec::sync_wait(stdexec::get_scheduler()).value();

    auto y = stdexec::let_value(stdexec::get_scheduler(), [](auto sched) {
        return stdexec::starts_on(sched, stdexec::then(stdexec::just(), [] {
                                      std::println("from run_loop");
                                      return 42;
                                  }));
    });
    stdexec::sync_wait(std::move(y));

    stdexec::sync_wait(
        stdexec::when_all(stdexec::just(42), stdexec::get_scheduler(), stdexec::get_stop_token())
    );
    return 0;
}
