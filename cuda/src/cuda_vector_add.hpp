#pragma once

#include <cstddef>
#include <string>

namespace cuda_demo {

    struct DeviceInfo {
        std::string name;
        int compute_major{ 0 };
        int compute_minor{ 0 };
    };

    struct VectorAddStats {
        double kernel_milliseconds{ 0.0 };
        double effective_bandwidth_gbps{ 0.0 };
        int threads_per_block{ 0 };
        int blocks_per_grid{ 0 };
    };

    [[nodiscard]] auto query_device() -> DeviceInfo;

    [[nodiscard]] auto vector_add(
        int const* a,
        int const* b,
        int* c,
        std::size_t n
    ) -> VectorAddStats;

} // namespace cuda_demo
