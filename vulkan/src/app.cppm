module;

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

export module vulkan:app;

import std;

import :common;
import :window;
import :device;
import :swap_chain;
import :pipeline;
import :renderer;

export namespace vulkan {

    std::filesystem::path executable_directory();

    class Application {
    public:
        explicit Application(std::filesystem::path shader_dir);
        ~Application();

        Application(Application const&) = delete;
        Application& operator=(Application const&) = delete;
        Application(Application&&) = delete;
        Application& operator=(Application&&) = delete;

        int Run();

    private:
        struct Impl;

        std::unique_ptr<Impl> m_impl;
    };

} // namespace vulkan

namespace vulkan {

    struct Application::Impl {
        Window window;
        VulkanContext context;
        std::unique_ptr<SwapChain> swap_chain;
        std::unique_ptr<GraphicsPipeline> pipeline;
        std::unique_ptr<Renderer> renderer;

        explicit Impl(std::filesystem::path shader_dir);
        ~Impl();

        int Run();
        void recreateSwapChain();
    };

    std::filesystem::path executable_directory()
    {
        std::array<wchar_t, 32768> path_buffer{};
        auto const length{ GetModuleFileNameW(nullptr, path_buffer.data(), static_cast<DWORD>(path_buffer.size())) };
        if (length == 0 || static_cast<std::size_t>(length) >= path_buffer.size()) {
            throw std::runtime_error("GetModuleFileNameW failed");
        }

        return std::filesystem::path{ path_buffer.data() }.parent_path();
    }

    Application::Application(std::filesystem::path shader_dir) : m_impl{ std::make_unique<Impl>(std::move(shader_dir)) } {}

    Application::~Application() {}

    int Application::Run()
    {
        return m_impl->Run();
    }

    Application::Impl::Impl(std::filesystem::path shader_dir)
        : window{ WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE },
          context{ window.Handle() }
    {
        auto const [width, height] = window.FramebufferSize();
        swap_chain = std::make_unique<SwapChain>(
            context.PhysicalDevice(),
            context.Device(),
            context.Surface(),
            context.GraphicsQueueFamily(),
            context.PresentQueueFamily(),
            static_cast<std::uint32_t>(width),
            static_cast<std::uint32_t>(height)
        );
        pipeline = std::make_unique<GraphicsPipeline>(context.Device(), swap_chain->ImageFormat(), shader_dir);
        swap_chain->CreateFramebuffers(pipeline->RenderPass());
        renderer = std::make_unique<Renderer>(context.Device(), context.GraphicsQueueFamily());
    }

    Application::Impl::~Impl()
    {
        if (context.Device() != VK_NULL_HANDLE) {
            context.WaitIdle();
        }

        renderer.reset();
        swap_chain.reset();
        pipeline.reset();
    }

    int Application::Impl::Run()
    {
        window.ClearResizeRequest();

        while (!window.ShouldClose()) {
            window.PollEvents();

            if (window.ResizeRequested()) {
                recreateSwapChain();
                if (window.ShouldClose()) {
                    break;
                }
            }

            auto const draw_result{ renderer->DrawFrame(context, *swap_chain, *pipeline) };
            if (draw_result == DrawResult::RecreateSwapChain || window.ResizeRequested()) {
                recreateSwapChain();
            }
        }

        context.WaitIdle();
        return 0;
    }

    void Application::Impl::recreateSwapChain()
    {
        auto [width, height] = window.FramebufferSize();
        while ((width == 0 || height == 0) && !window.ShouldClose()) {
            window.WaitEvents();
            std::tie(width, height) = window.FramebufferSize();
        }

        if (width == 0 || height == 0) {
            return;
        }

        window.ClearResizeRequest();
        context.WaitIdle();
        swap_chain->Recreate(static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height), pipeline->RenderPass());
    }

} // namespace vulkan
