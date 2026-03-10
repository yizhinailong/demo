#include <iostream>

#include "ScopeProfiler.hpp"

int main() {
    {
        utils::ScopeProfiler _("Test");
        std::cout << "hello world!" << std::endl;
    }
    utils::ScopeProfiler::PrintLog();

    return 0;
}
