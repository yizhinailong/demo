module;

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

module vulkan:device;

import std;

import :common;

namespace vulkan {

    class VulkanContext {
    public:
        explicit VulkanContext(GLFWwindow* window);
        ~VulkanContext();

        VulkanContext(VulkanContext const&) = delete;
        VulkanContext& operator=(VulkanContext const&) = delete;
        VulkanContext(VulkanContext&&) = delete;
        VulkanContext& operator=(VulkanContext&&) = delete;

        [[nodiscard]] VkSurfaceKHR Surface() const noexcept;
        [[nodiscard]] VkPhysicalDevice PhysicalDevice() const noexcept;
        [[nodiscard]] VkDevice Device() const noexcept;
        [[nodiscard]] VkQueue GraphicsQueue() const noexcept;
        [[nodiscard]] VkQueue PresentQueue() const noexcept;
        [[nodiscard]] std::uint32_t GraphicsQueueFamily() const noexcept;
        [[nodiscard]] std::uint32_t PresentQueueFamily() const noexcept;

        void WaitIdle() const;

    private:
        bool m_validation_enabled{ false };
        VkInstance m_instance{ VK_NULL_HANDLE };
        VkDebugUtilsMessengerEXT m_debug_messenger{ VK_NULL_HANDLE };
        VkSurfaceKHR m_surface{ VK_NULL_HANDLE };
        VkPhysicalDevice m_physical_device{ VK_NULL_HANDLE };
        VkDevice m_device{ VK_NULL_HANDLE };
        std::uint32_t m_graphics_queue_family{ 0 };
        std::uint32_t m_present_queue_family{ 0 };
        VkQueue m_graphics_queue{ VK_NULL_HANDLE };
        VkQueue m_present_queue{ VK_NULL_HANDLE };

        void createInstance();
        void setupDebugMessenger();
        void createSurface(GLFWwindow* window);
        void pickPhysicalDevice();
        void createLogicalDevice();
        [[nodiscard]] bool checkValidationLayerSupport() const;
        [[nodiscard]] std::vector<char const*> getRequiredExtensions() const;
        [[nodiscard]] bool isDeviceSuitable(VkPhysicalDevice device) const;
    };

    VulkanContext::VulkanContext(GLFWwindow* window)
    {
        createInstance();
        setupDebugMessenger();
        createSurface(window);
        pickPhysicalDevice();
        createLogicalDevice();
    }

    VulkanContext::~VulkanContext()
    {
        if (m_device != VK_NULL_HANDLE) {
            vkDeviceWaitIdle(m_device);
            vkDestroyDevice(m_device, nullptr);
        }

        if (m_debug_messenger != VK_NULL_HANDLE) {
            auto const destroy_debug_messenger{ reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT")) };
            if (destroy_debug_messenger != nullptr) {
                destroy_debug_messenger(m_instance, m_debug_messenger, nullptr);
            }
        }

        if (m_surface != VK_NULL_HANDLE) {
            vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
        }

        if (m_instance != VK_NULL_HANDLE) {
            vkDestroyInstance(m_instance, nullptr);
        }
    }

    VkSurfaceKHR VulkanContext::Surface() const noexcept
    {
        return m_surface;
    }

    VkPhysicalDevice VulkanContext::PhysicalDevice() const noexcept
    {
        return m_physical_device;
    }

    VkDevice VulkanContext::Device() const noexcept
    {
        return m_device;
    }

    VkQueue VulkanContext::GraphicsQueue() const noexcept
    {
        return m_graphics_queue;
    }

    VkQueue VulkanContext::PresentQueue() const noexcept
    {
        return m_present_queue;
    }

    std::uint32_t VulkanContext::GraphicsQueueFamily() const noexcept
    {
        return m_graphics_queue_family;
    }

    std::uint32_t VulkanContext::PresentQueueFamily() const noexcept
    {
        return m_present_queue_family;
    }

    void VulkanContext::WaitIdle() const
    {
        check(vkDeviceWaitIdle(m_device), "vkDeviceWaitIdle");
    }

    void VulkanContext::createInstance()
    {
        m_validation_enabled = checkValidationLayerSupport();

        if (!m_validation_enabled) {
            std::cout << "VK_LAYER_KHRONOS_validation is not available; running without validation layers.\n";
            std::cout << "Install or register the Khronos validation layer to enable it.\n";
        }

        VkApplicationInfo application_info{};
        application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        application_info.pApplicationName = "Vulkan Triangle";
        application_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        application_info.pEngineName = "No Engine";
        application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        application_info.apiVersion = VK_API_VERSION_1_3;

        auto const extensions{ getRequiredExtensions() };

        VkInstanceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.pApplicationInfo = &application_info;
        create_info.enabledExtensionCount = static_cast<std::uint32_t>(extensions.size());
        create_info.ppEnabledExtensionNames = extensions.data();

        auto debug_create_info{ make_debug_messenger_create_info() };
        if (m_validation_enabled) {
            create_info.enabledLayerCount = static_cast<std::uint32_t>(VALIDATION_LAYERS.size());
            create_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();
            create_info.pNext = &debug_create_info;
        }

        check(vkCreateInstance(&create_info, nullptr, &m_instance), "vkCreateInstance");
    }

    void VulkanContext::setupDebugMessenger()
    {
        if (!m_validation_enabled) {
            return;
        }

        auto create_info{ make_debug_messenger_create_info() };
        auto const create_debug_messenger{ reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT")) };
        if (create_debug_messenger == nullptr) {
            throw std::runtime_error("vkCreateDebugUtilsMessengerEXT is not available");
        }

        check(create_debug_messenger(m_instance, &create_info, nullptr, &m_debug_messenger), "vkCreateDebugUtilsMessengerEXT");
    }

    void VulkanContext::createSurface(GLFWwindow* window)
    {
        check(glfwCreateWindowSurface(m_instance, window, nullptr, &m_surface), "glfwCreateWindowSurface");
    }

    void VulkanContext::pickPhysicalDevice()
    {
        std::uint32_t device_count{ 0 };
        check(vkEnumeratePhysicalDevices(m_instance, &device_count, nullptr), "vkEnumeratePhysicalDevices");
        if (device_count == 0) {
            throw std::runtime_error("No Vulkan-capable physical devices were found");
        }

        std::vector<VkPhysicalDevice> devices(device_count);
        check(vkEnumeratePhysicalDevices(m_instance, &device_count, devices.data()), "vkEnumeratePhysicalDevices");

        for (auto const device : devices) {
            if (isDeviceSuitable(device)) {
                m_physical_device = device;
                break;
            }
        }

        if (m_physical_device == VK_NULL_HANDLE) {
            throw std::runtime_error("No suitable Vulkan device was found");
        }

        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(m_physical_device, &properties);
        std::cout << "Using GPU: " << properties.deviceName << '\n';
    }

    void VulkanContext::createLogicalDevice()
    {
        auto const indices{ find_queue_families(m_physical_device, m_surface) };
        m_graphics_queue_family = indices.graphics_family.value();
        m_present_queue_family = indices.present_family.value();

        std::vector<std::uint32_t> unique_families{ m_graphics_queue_family };
        if (m_present_queue_family != m_graphics_queue_family) {
            unique_families.push_back(m_present_queue_family);
        }

        float const queue_priority{ 1.0F };
        std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
        queue_create_infos.reserve(unique_families.size());
        for (auto const family : unique_families) {
            VkDeviceQueueCreateInfo queue_create_info{};
            queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_create_info.queueFamilyIndex = family;
            queue_create_info.queueCount = 1;
            queue_create_info.pQueuePriorities = &queue_priority;
            queue_create_infos.push_back(queue_create_info);
        }

        VkPhysicalDeviceFeatures device_features{};

        VkDeviceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.queueCreateInfoCount = static_cast<std::uint32_t>(queue_create_infos.size());
        create_info.pQueueCreateInfos = queue_create_infos.data();
        create_info.pEnabledFeatures = &device_features;
        create_info.enabledExtensionCount = static_cast<std::uint32_t>(DEVICE_EXTENSIONS.size());
        create_info.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();

        check(vkCreateDevice(m_physical_device, &create_info, nullptr, &m_device), "vkCreateDevice");

        vkGetDeviceQueue(m_device, m_graphics_queue_family, 0, &m_graphics_queue);
        vkGetDeviceQueue(m_device, m_present_queue_family, 0, &m_present_queue);
    }

    bool VulkanContext::checkValidationLayerSupport() const
    {
        std::uint32_t layer_count{ 0 };
        check(vkEnumerateInstanceLayerProperties(&layer_count, nullptr), "vkEnumerateInstanceLayerProperties");

        std::vector<VkLayerProperties> available_layers(layer_count);
        check(vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data()), "vkEnumerateInstanceLayerProperties");

        for (auto const* layer_name : VALIDATION_LAYERS) {
            bool found{ false };
            for (auto const& properties : available_layers) {
                if (std::strcmp(layer_name, properties.layerName) == 0) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                return false;
            }
        }

        return true;
    }

    std::vector<char const*> VulkanContext::getRequiredExtensions() const
    {
        std::uint32_t extension_count{ 0 };
        auto const* glfw_extensions{ glfwGetRequiredInstanceExtensions(&extension_count) };
        if (glfw_extensions == nullptr) {
            throw std::runtime_error("glfwGetRequiredInstanceExtensions failed");
        }

        std::vector<char const*> extensions(glfw_extensions, glfw_extensions + extension_count);
        if (m_validation_enabled) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        return extensions;
    }

    bool VulkanContext::isDeviceSuitable(VkPhysicalDevice device) const
    {
        auto const indices{ find_queue_families(device, m_surface) };
        if (!indices.IsComplete() || !check_device_extension_support(device)) {
            return false;
        }

        auto const support{ query_swap_chain_support(device, m_surface) };
        return !support.formats.empty() && !support.present_modes.empty();
    }

} // namespace vulkan
