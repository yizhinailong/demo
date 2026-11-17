#include <cstdio>
#include <cstdlib>
#include <exception>
#include <vector>

#include "cuda_vector_add.hpp"

auto main() -> int {
    try {
        constexpr std::size_t ELEMENT_COUNT{ 1U << 20 };

        cuda_demo::DeviceInfo const device{ cuda_demo::query_device() };
        std::printf(
            "Device: %s (sm_%d%d)\n",
            device.name.c_str(),
            device.compute_major,
            device.compute_minor
        );

        std::vector<int> a(ELEMENT_COUNT);
        std::vector<int> b(ELEMENT_COUNT);
        std::vector<int> c(ELEMENT_COUNT, 0);

        for (std::size_t i{ 0 }; i < ELEMENT_COUNT; ++i) {
            a[i] = static_cast<int>(i);
            b[i] = static_cast<int>(2 * i);
        }

        cuda_demo::VectorAddStats const stats{
            cuda_demo::vector_add(a.data(), b.data(), c.data(), ELEMENT_COUNT)
        };

        std::printf(
            "n=%zu, threads=%d, blocks=%d\n",
            ELEMENT_COUNT,
            stats.threads_per_block,
            stats.blocks_per_grid
        );
        std::printf("Kernel time: %.3f ms\n", stats.kernel_milliseconds);
        std::printf("Effective bandwidth: %.1f GB/s\n", stats.effective_bandwidth_gbps);

        for (std::size_t i{ 0 }; i < ELEMENT_COUNT; ++i) {
            int const expected{ a[i] + b[i] };
            if (c[i] != expected) {
                std::fprintf(
                    stderr,
                    "Mismatch at index %zu: expected %d, got %d\n",
                    i,
                    expected,
                    c[i]
                );
                return EXIT_FAILURE;
            }
        }

        std::printf("PASS (%zu elements)\n", ELEMENT_COUNT);
        return EXIT_SUCCESS;
    } catch (std::exception const& error) {
        std::fprintf(stderr, "CUDA demo failed: %s\n", error.what());
        return EXIT_FAILURE;
    }
}
