#include <algorithm>
#include <cstddef>
#include <limits>
#include <source_location>
#include <stdexcept>
#include <string>

#include <cuda_runtime.h>

#include "cuda_vector_add.hpp"

namespace {

    constexpr int THREADS_PER_BLOCK{ 256 };
    constexpr int MAX_BLOCKS{ 4096 };

    auto check_cuda(
        cudaError_t status,
        char const* expression,
        std::source_location location = std::source_location::current()
    ) -> void {
        if (status == cudaSuccess) {
            return;
        }

        throw std::runtime_error(
            std::string{ location.file_name() } + ':' +
            std::to_string(static_cast<unsigned long>(location.line())) + ' ' + expression +
            " failed: " + cudaGetErrorString(status)
        );
    }

    template <typename T>
    class DeviceBuffer {
    public:
        explicit DeviceBuffer(std::size_t count) {
            check_cuda(
                cudaMalloc(reinterpret_cast<void**>(&m_data), count * sizeof(T)),
                "cudaMalloc"
            );
        }

        ~DeviceBuffer() {
            if (m_data != nullptr) {
                cudaFree(m_data);
            }
        }

        DeviceBuffer(DeviceBuffer const&) = delete;
        DeviceBuffer& operator=(DeviceBuffer const&) = delete;

        [[nodiscard]] auto Data() const noexcept -> T* {
            return m_data;
        }

    private:
        T* m_data{ nullptr };
    };

    class CudaEvent {
    public:
        CudaEvent() {
            check_cuda(cudaEventCreate(&m_event), "cudaEventCreate");
        }

        ~CudaEvent() {
            if (m_event != nullptr) {
                cudaEventDestroy(m_event);
            }
        }

        CudaEvent(CudaEvent const&) = delete;
        CudaEvent& operator=(CudaEvent const&) = delete;

        auto Record() const -> void {
            check_cuda(cudaEventRecord(m_event), "cudaEventRecord");
        }

        [[nodiscard]] auto Handle() const noexcept -> cudaEvent_t {
            return m_event;
        }

    private:
        cudaEvent_t m_event{ nullptr };
    };

    __global__ void vector_add_kernel(int const* a, int const* b, int* c, int n) {
        for (int i = blockIdx.x * blockDim.x + threadIdx.x; i < n;
             i += blockDim.x * gridDim.x) {
            c[i] = a[i] + b[i];
        }
    }

} // namespace

namespace cuda_demo {

    auto query_device() -> DeviceInfo {
        int device{ 0 };
        check_cuda(cudaGetDevice(&device), "cudaGetDevice");

        cudaDeviceProp properties{};
        check_cuda(cudaGetDeviceProperties(&properties, device), "cudaGetDeviceProperties");

        return DeviceInfo{ std::string{ properties.name }, properties.major, properties.minor };
    }

    auto vector_add(
        int const* a,
        int const* b,
        int* c,
        std::size_t n
    ) -> VectorAddStats {
        if (n == 0) {
            return VectorAddStats{};
        }
        if (n > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
            throw std::invalid_argument("vector_add: element count exceeds int range");
        }

        DeviceBuffer<int> device_a{ n };
        DeviceBuffer<int> device_b{ n };
        DeviceBuffer<int> device_c{ n };

        check_cuda(
            cudaMemcpy(device_a.Data(), a, n * sizeof(int), cudaMemcpyHostToDevice),
            "cudaMemcpy"
        );
        check_cuda(
            cudaMemcpy(device_b.Data(), b, n * sizeof(int), cudaMemcpyHostToDevice),
            "cudaMemcpy"
        );

        int const threads{ THREADS_PER_BLOCK };
        int const blocks{
            std::min(static_cast<int>((n + threads - 1) / threads), MAX_BLOCKS)
        };
        int const element_count{ static_cast<int>(n) };

        vector_add_kernel<<<blocks, threads>>>(
            device_a.Data(),
            device_b.Data(),
            device_c.Data(),
            element_count
        );
        check_cuda(cudaGetLastError(), "cudaGetLastError");
        check_cuda(cudaDeviceSynchronize(), "cudaDeviceSynchronize");

        CudaEvent start;
        CudaEvent stop;

        start.Record();
        vector_add_kernel<<<blocks, threads>>>(
            device_a.Data(),
            device_b.Data(),
            device_c.Data(),
            element_count
        );
        check_cuda(cudaGetLastError(), "cudaGetLastError");
        stop.Record();
        check_cuda(cudaEventSynchronize(stop.Handle()), "cudaEventSynchronize");

        float kernel_milliseconds{ 0.0F };
        check_cuda(
            cudaEventElapsedTime(&kernel_milliseconds, start.Handle(), stop.Handle()),
            "cudaEventElapsedTime"
        );

        check_cuda(
            cudaMemcpy(c, device_c.Data(), n * sizeof(int), cudaMemcpyDeviceToHost),
            "cudaMemcpy"
        );

        double const elapsed_milliseconds{ static_cast<double>(kernel_milliseconds) };
        double const bandwidth_gbps{
            elapsed_milliseconds > 0.0 ? 3.0 * static_cast<double>(n) * sizeof(int) / (elapsed_milliseconds * 1.0e6) : 0.0
        };

        return VectorAddStats{
            elapsed_milliseconds,
            bandwidth_gbps,
            threads,
            blocks
        };
    }

} // namespace cuda_demo
