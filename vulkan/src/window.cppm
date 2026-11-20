module;

#include <GLFW/glfw3.h>

module vulkan:window;

import std;

namespace vulkan {

    class Window {
    public:
        Window(std::uint32_t width, std::uint32_t height, char const* title);
        ~Window();

        Window(Window const&) = delete;
        Window& operator=(Window const&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;

        [[nodiscard]] GLFWwindow* Handle() const noexcept;
        [[nodiscard]] bool ShouldClose() const;
        [[nodiscard]] std::pair<int, int> FramebufferSize() const;
        [[nodiscard]] bool ResizeRequested() const noexcept;

        void ClearResizeRequest() noexcept;
        void PollEvents() const;
        void WaitEvents() const;

    private:
        inline static bool s_framebuffer_resized{ false };
        GLFWwindow* m_window{ nullptr };
        bool m_glfw_initialized{ false };

        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    };

    Window::Window(std::uint32_t width, std::uint32_t height, char const* title)
    {
        if (glfwInit() != GLFW_TRUE) {
            throw std::runtime_error("glfwInit failed");
        }
        m_glfw_initialized = true;

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_window = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), title, nullptr, nullptr);
        if (m_window == nullptr) {
            glfwTerminate();
            m_glfw_initialized = false;
            throw std::runtime_error("glfwCreateWindow failed");
        }

        glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
    }

    Window::~Window()
    {
        if (m_window != nullptr) {
            glfwDestroyWindow(m_window);
        }

        if (m_glfw_initialized) {
            glfwTerminate();
        }
    }

    GLFWwindow* Window::Handle() const noexcept
    {
        return m_window;
    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(m_window) != 0;
    }

    std::pair<int, int> Window::FramebufferSize() const
    {
        int width{ 0 };
        int height{ 0 };
        glfwGetFramebufferSize(m_window, &width, &height);
        return { width, height };
    }

    bool Window::ResizeRequested() const noexcept
    {
        return s_framebuffer_resized;
    }

    void Window::ClearResizeRequest() noexcept
    {
        s_framebuffer_resized = false;
    }

    void Window::PollEvents() const
    {
        glfwPollEvents();
    }

    void Window::WaitEvents() const
    {
        glfwWaitEvents();
    }

    void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
    {
        (void)window;
        (void)width;
        (void)height;
        s_framebuffer_resized = true;
    }

} // namespace vulkan
