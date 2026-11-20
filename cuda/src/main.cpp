#include <cstdio>
#include <cstdlib>
#include <exception>
#include <print>
#include <vector>

#include "cuda_vector_add.hpp"

int main()
{
    try {
        constexpr std::size_t ELEMENT_COUNT{ 1U << 20 };

        cuda_demo::DeviceInfo const device{ cuda_demo::query_device() };
        std::println(
            "Device: {} (sm_{}{})",
            device.name,
            device.compute_major,
            device.compute_minor);

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

        std::println(
            "n={}, threads={}, blocks={}",
            ELEMENT_COUNT,
            stats.threads_per_block,
            stats.blocks_per_grid);
        std::println("Kernel time: {:.3f} ms", stats.kernel_milliseconds);
        std::println("Effective bandwidth: {:.1f} GB/s", stats.effective_bandwidth_gbps);

        for (std::size_t i{ 0 }; i < ELEMENT_COUNT; ++i) {
            int const expected{ a[i] + b[i] };
            if (c[i] != expected) {
                std::println(
                    stderr,
                    "Mismatch at index {}: expected {}, got {}",
                    i,
                    expected,
                    c[i]);
                return EXIT_FAILURE;
            }
        }

        std::println("PASS ({} elements)", ELEMENT_COUNT);
        return EXIT_SUCCESS;
    } catch (std::exception const& error) {
        std::println(stderr, "CUDA demo failed: {}", error.what());
        return EXIT_FAILURE;
    }
}
