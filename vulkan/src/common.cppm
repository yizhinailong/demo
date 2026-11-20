module;

#include <vulkan/vulkan.h>

module vulkan:common;

import std;

namespace vulkan {

    constexpr std::uint32_t WINDOW_WIDTH{ 1280 };
    constexpr std::uint32_t WINDOW_HEIGHT{ 720 };
    constexpr std::uint32_t MAX_FRAMES_IN_FLIGHT{ 2 };
    constexpr char const* WINDOW_TITLE{ "Vulkan Triangle" };
    constexpr std::array<char const*, 1> VALIDATION_LAYERS{ "VK_LAYER_KHRONOS_validation" };
    constexpr std::array<char const*, 1> DEVICE_EXTENSIONS{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    struct QueueFamilyIndices {
        std::optional<std::uint32_t> graphics_family;
        std::optional<std::uint32_t> present_family;

        [[nodiscard]] bool IsComplete() const
        {
            return graphics_family.has_value() && present_family.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities{};
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> present_modes;
    };

    void check(VkResult result, char const* operation)
    {
        if (result != VK_SUCCESS) {
            throw std::runtime_error(std::string{ operation } + " failed with VkResult " + std::to_string(static_cast<int>(result)));
        }
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT severity,
        VkDebugUtilsMessageTypeFlagsEXT type,
        VkDebugUtilsMessengerCallbackDataEXT const* data,
        void* user_data
    )
    {
        (void)severity;
        (void)type;
        (void)user_data;
        std::cerr << "[Vulkan] " << data->pMessage << '\n';
        return VK_FALSE;
    }

    VkDebugUtilsMessengerCreateInfoEXT make_debug_messenger_create_info()
    {
        VkDebugUtilsMessengerCreateInfoEXT create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        create_info.pfnUserCallback = debug_callback;
        return create_info;
    }

    std::vector<char> read_file(std::filesystem::path const& file_path)
    {
        std::ifstream file{ file_path, std::ios::ate | std::ios::binary };
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open shader file: " + file_path.string());
        }

        auto const file_size{ static_cast<std::size_t>(file.tellg()) };
        std::vector<char> buffer(file_size);
        file.seekg(0);
        file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
        if (!file) {
            throw std::runtime_error("Failed to read shader file: " + file_path.string());
        }

        return buffer;
    }

    QueueFamilyIndices find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        QueueFamilyIndices indices;

        std::uint32_t family_count{ 0 };
        vkGetPhysicalDeviceQueueFamilyProperties(device, &family_count, nullptr);
        std::vector<VkQueueFamilyProperties> families(family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &family_count, families.data());

        for (std::uint32_t index{ 0 }; index < family_count; ++index) {
            if ((families[index].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
                indices.graphics_family = index;
            }

            VkBool32 present_support{ VK_FALSE };
            check(vkGetPhysicalDeviceSurfaceSupportKHR(device, index, surface, &present_support), "vkGetPhysicalDeviceSurfaceSupportKHR");
            if (present_support == VK_TRUE) {
                indices.present_family = index;
            }

            if (indices.IsComplete()) {
                break;
            }
        }

        return indices;
    }

    SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        SwapChainSupportDetails details;

        check(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities), "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");

        std::uint32_t format_count{ 0 };
        check(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr), "vkGetPhysicalDeviceSurfaceFormatsKHR");
        if (format_count != 0) {
            details.formats.resize(format_count);
            check(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, details.formats.data()), "vkGetPhysicalDeviceSurfaceFormatsKHR");
        }

        std::uint32_t present_mode_count{ 0 };
        check(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, nullptr), "vkGetPhysicalDeviceSurfacePresentModesKHR");
        if (present_mode_count != 0) {
            details.present_modes.resize(present_mode_count);
            check(vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, details.present_modes.data()), "vkGetPhysicalDeviceSurfacePresentModesKHR");
        }

        return details;
    }

    bool check_device_extension_support(VkPhysicalDevice device)
    {
        std::uint32_t extension_count{ 0 };
        check(vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr), "vkEnumerateDeviceExtensionProperties");

        std::vector<VkExtensionProperties> available_extensions(extension_count);
        check(vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data()), "vkEnumerateDeviceExtensionProperties");

        for (auto const* required : DEVICE_EXTENSIONS) {
            bool found{ false };
            for (auto const& extension : available_extensions) {
                if (std::strcmp(required, extension.extensionName) == 0) {
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

    VkSurfaceFormatKHR choose_swap_surface_format(std::vector<VkSurfaceFormatKHR> const& formats)
    {
        for (auto const& format : formats) {
            if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return format;
            }
        }

        return formats.front();
    }

    VkPresentModeKHR choose_swap_present_mode(std::vector<VkPresentModeKHR> const& present_modes)
    {
        for (auto const mode : present_modes) {
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return mode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D choose_swap_extent(
        VkSurfaceCapabilitiesKHR const& capabilities,
        std::uint32_t width,
        std::uint32_t height
    )
    {
        if (capabilities.currentExtent.width != std::numeric_limits<std::uint32_t>::max()) {
            return capabilities.currentExtent;
        }

        VkExtent2D actual_extent{ width, height };
        actual_extent.width = std::clamp(actual_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actual_extent.height = std::clamp(actual_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        return actual_extent;
    }

} // namespace vulkan
