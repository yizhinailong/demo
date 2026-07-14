import std;

namespace n709c {
    struct executor
    {
        void execute(std::function<void(void)> const& task)
        {
            threads.push_back(std::thread([task]() { 
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(250ms);

                task(); 
            }));
        }

        ~executor()
        {
            for (auto& t : threads)
                t.join();
        }
    private:
        std::vector<std::thread> threads;
    };

    struct building : std::enable_shared_from_this<building>
    {
        building() { std::cout << "building created\n"; }
        ~building() { std::cout << "building destroyed\n"; }

        void upgrade()
        {
            if (exec)
            {
                exec->execute([self = shared_from_this()]() {
                    self->do_upgrade();
                });
            }
        }      

        void set_executor(executor* e)
        {
            exec = e;
        }
    private:
        void do_upgrade()
        {
            std::cout << "upgrading\n";
            operational = false;

            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1000ms);

            operational = true;
            std::cout << "building is functional\n";
        }

        bool operational = false;
        executor* exec = nullptr;
    };
} // namespace n709c

auto main() -> int {
    {
        using namespace n709c;
        
        executor e;
        std::shared_ptr<building> b = std::make_shared<building>();
        b->set_executor(&e);
        b->upgrade();
        
        std::cout << "main finished\n";
    }
    return 0;
}
