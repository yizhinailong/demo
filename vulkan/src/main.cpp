#define GLFW_INCLUDE_VULKAN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <GLFW/glfw3.h>
#include <windows.h>

namespace {

    constexpr std::uint32_t WINDOW_WIDTH{ 1280 };
    constexpr std::uint32_t WINDOW_HEIGHT{ 720 };
    constexpr std::uint32_t MAX_FRAMES_IN_FLIGHT{ 2 };
    constexpr char const* WINDOW_TITLE{ "Vulkan Triangle" };
    constexpr std::array<char const*, 1> VALIDATION_LAYERS{ "VK_LAYER_KHRONOS_validation" };
    constexpr std::array<char const*, 1> DEVICE_EXTENSIONS{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    bool g_framebuffer_resized{ false };

    auto framebuffer_resize_callback(GLFWwindow* window, int width, int height) -> void
    {
        (void)window;
        (void)width;
        (void)height;
        g_framebuffer_resized = true;
    }

    auto check(VkResult result, char const* operation) -> void
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

    auto make_debug_messenger_create_info() -> VkDebugUtilsMessengerCreateInfoEXT
    {
        VkDebugUtilsMessengerCreateInfoEXT create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        create_info.pfnUserCallback = debug_callback;
        return create_info;
    }

    auto read_file(std::filesystem::path const& path) -> std::vector<char>
    {
        std::ifstream file{ path, std::ios::ate | std::ios::binary };
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open shader file: " + path.string());
        }

        auto const file_size{ static_cast<std::size_t>(file.tellg()) };
        std::vector<char> buffer(file_size);
        file.seekg(0);
        file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
        if (!file) {
            throw std::runtime_error("Failed to read shader file: " + path.string());
        }

        return buffer;
    }

    auto executable_directory() -> std::filesystem::path
    {
        std::array<wchar_t, 32768> path_buffer{};
        auto const length{ GetModuleFileNameW(nullptr, path_buffer.data(), static_cast<DWORD>(path_buffer.size())) };
        if (length == 0 || static_cast<std::size_t>(length) >= path_buffer.size()) {
            throw std::runtime_error("GetModuleFileNameW failed");
        }

        return std::filesystem::path{ path_buffer.data() }.parent_path();
    }

    class Application {
    public:
        explicit Application(std::filesystem::path shader_dir);
        ~Application();

        Application(Application const&) = delete;
        auto operator=(Application const&) -> Application& = delete;
        Application(Application&&) = delete;
        auto operator=(Application&&) -> Application& = delete;

        auto Run() -> int;

    private:
        struct QueueFamilyIndices {
            std::optional<std::uint32_t> graphics_family;
            std::optional<std::uint32_t> present_family;

            [[nodiscard]] auto isComplete() const -> bool
            {
                return graphics_family.has_value() && present_family.has_value();
            }
        };

        struct SwapChainSupportDetails {
            VkSurfaceCapabilitiesKHR capabilities{};
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> present_modes;
        };

        auto createWindow() -> void;
        auto createInstance() -> void;
        auto setupDebugMessenger() -> void;
        auto createSurface() -> void;
        auto pickPhysicalDevice() -> void;
        auto createLogicalDevice() -> void;
        auto createSwapChain() -> void;
        auto createImageViews() -> void;
        auto createRenderPass() -> void;
        auto createGraphicsPipeline() -> void;
        auto createFramebuffers() -> void;
        auto createCommandPool() -> void;
        auto createCommandBuffers() -> void;
        auto createRenderFinishedSemaphores() -> void;
        auto createSyncObjects() -> void;
        auto drawFrame() -> void;
        auto recreateSwapChain() -> void;
        auto cleanupSwapChain() -> void;

        auto loadShader(char const* file_name) const -> std::vector<char>;
        auto checkValidationLayerSupport() const -> bool;
        auto getRequiredExtensions() const -> std::vector<char const*>;
        auto findQueueFamilies(VkPhysicalDevice device) const -> QueueFamilyIndices;
        auto querySwapChainSupport(VkPhysicalDevice device) const -> SwapChainSupportDetails;
        auto checkDeviceExtensionSupport(VkPhysicalDevice device) const -> bool;
        auto isDeviceSuitable(VkPhysicalDevice device) const -> bool;
        auto chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> const& formats) const -> VkSurfaceFormatKHR;
        auto chooseSwapPresentMode(std::vector<VkPresentModeKHR> const& present_modes) const -> VkPresentModeKHR;
        auto chooseSwapExtent(VkSurfaceCapabilitiesKHR const& capabilities) const -> VkExtent2D;
        auto createShaderModule(std::vector<char> const& code) const -> VkShaderModule;
        auto recordCommandBuffer(VkCommandBuffer command_buffer, std::uint32_t image_index) const -> void;

        GLFWwindow* m_window{ nullptr };
        bool m_glfw_initialized{ false };
        std::filesystem::path m_shader_dir;
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

        VkSwapchainKHR m_swap_chain{ VK_NULL_HANDLE };
        std::vector<VkImage> m_swap_chain_images;
        VkFormat m_swap_chain_image_format{ VK_FORMAT_UNDEFINED };
        VkExtent2D m_swap_chain_extent{};
        std::vector<VkImageView> m_swap_chain_image_views;

        VkRenderPass m_render_pass{ VK_NULL_HANDLE };
        VkPipelineLayout m_pipeline_layout{ VK_NULL_HANDLE };
        VkPipeline m_graphics_pipeline{ VK_NULL_HANDLE };
        std::vector<VkFramebuffer> m_swap_chain_framebuffers;

        VkCommandPool m_command_pool{ VK_NULL_HANDLE };
        std::vector<VkCommandBuffer> m_command_buffers;
        std::vector<VkSemaphore> m_image_available_semaphores;
        std::vector<VkSemaphore> m_render_finished_semaphores;
        std::vector<VkFence> m_in_flight_fences;
        std::uint32_t m_current_frame{ 0 };
    };

    Application::Application(std::filesystem::path shader_dir) : m_shader_dir{ std::move(shader_dir) } {}

    Application::~Application()
    {
        if (m_device != VK_NULL_HANDLE) {
            vkDeviceWaitIdle(m_device);
            cleanupSwapChain();

            for (auto const semaphore : m_image_available_semaphores) {
                if (semaphore != VK_NULL_HANDLE) {
                    vkDestroySemaphore(m_device, semaphore, nullptr);
                }
            }

            for (auto const fence : m_in_flight_fences) {
                if (fence != VK_NULL_HANDLE) {
                    vkDestroyFence(m_device, fence, nullptr);
                }
            }

            if (m_command_pool != VK_NULL_HANDLE) {
                vkDestroyCommandPool(m_device, m_command_pool, nullptr);
            }

            if (m_graphics_pipeline != VK_NULL_HANDLE) {
                vkDestroyPipeline(m_device, m_graphics_pipeline, nullptr);
            }

            if (m_pipeline_layout != VK_NULL_HANDLE) {
                vkDestroyPipelineLayout(m_device, m_pipeline_layout, nullptr);
            }

            if (m_render_pass != VK_NULL_HANDLE) {
                vkDestroyRenderPass(m_device, m_render_pass, nullptr);
            }

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

        if (m_window != nullptr) {
            glfwDestroyWindow(m_window);
        }

        if (m_glfw_initialized) {
            glfwTerminate();
        }
    }

    auto Application::Run() -> int
    {
        createWindow();
        createInstance();
        setupDebugMessenger();
        createSurface();
        pickPhysicalDevice();
        createLogicalDevice();
        createSwapChain();
        createRenderFinishedSemaphores();
        createImageViews();
        createRenderPass();
        createGraphicsPipeline();
        createFramebuffers();
        createCommandPool();
        createCommandBuffers();
        createSyncObjects();
        g_framebuffer_resized = false;

        while (glfwWindowShouldClose(m_window) == 0) {
            glfwPollEvents();

            if (g_framebuffer_resized) {
                recreateSwapChain();
                if (glfwWindowShouldClose(m_window) != 0) {
                    break;
                }
            }

            drawFrame();
        }

        check(vkDeviceWaitIdle(m_device), "vkDeviceWaitIdle");
        return 0;
    }

    auto Application::createWindow() -> void
    {
        if (glfwInit() != GLFW_TRUE) {
            throw std::runtime_error("glfwInit failed");
        }
        m_glfw_initialized = true;

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_window = glfwCreateWindow(
            static_cast<int>(WINDOW_WIDTH),
            static_cast<int>(WINDOW_HEIGHT),
            WINDOW_TITLE,
            nullptr,
            nullptr
        );
        if (m_window == nullptr) {
            throw std::runtime_error("glfwCreateWindow failed");
        }

        glfwSetFramebufferSizeCallback(m_window, framebuffer_resize_callback);
    }

    auto Application::createInstance() -> void
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

    auto Application::setupDebugMessenger() -> void
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

    auto Application::createSurface() -> void
    {
        check(glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface), "glfwCreateWindowSurface");
    }

    auto Application::pickPhysicalDevice() -> void
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

    auto Application::createLogicalDevice() -> void
    {
        auto const indices{ findQueueFamilies(m_physical_device) };
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

    auto Application::createSwapChain() -> void
    {
        auto const support{ querySwapChainSupport(m_physical_device) };
        auto const surface_format{ chooseSwapSurfaceFormat(support.formats) };
        auto const present_mode{ chooseSwapPresentMode(support.present_modes) };
        auto const extent{ chooseSwapExtent(support.capabilities) };

        std::uint32_t image_count{ support.capabilities.minImageCount + 1 };
        if (support.capabilities.maxImageCount > 0 && image_count > support.capabilities.maxImageCount) {
            image_count = support.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        create_info.surface = m_surface;
        create_info.minImageCount = image_count;
        create_info.imageFormat = surface_format.format;
        create_info.imageColorSpace = surface_format.colorSpace;
        create_info.imageExtent = extent;
        create_info.imageArrayLayers = 1;
        create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        std::array<std::uint32_t, 2> queue_family_indices{ m_graphics_queue_family, m_present_queue_family };
        if (m_graphics_queue_family != m_present_queue_family) {
            create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            create_info.queueFamilyIndexCount = static_cast<std::uint32_t>(queue_family_indices.size());
            create_info.pQueueFamilyIndices = queue_family_indices.data();
        } else {
            create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        create_info.preTransform = support.capabilities.currentTransform;
        create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        create_info.presentMode = present_mode;
        create_info.clipped = VK_TRUE;

        check(vkCreateSwapchainKHR(m_device, &create_info, nullptr, &m_swap_chain), "vkCreateSwapchainKHR");

        std::uint32_t swap_chain_image_count{ 0 };
        check(vkGetSwapchainImagesKHR(m_device, m_swap_chain, &swap_chain_image_count, nullptr), "vkGetSwapchainImagesKHR");
        m_swap_chain_images.resize(swap_chain_image_count);
        check(vkGetSwapchainImagesKHR(m_device, m_swap_chain, &swap_chain_image_count, m_swap_chain_images.data()), "vkGetSwapchainImagesKHR");

        m_swap_chain_image_format = surface_format.format;
        m_swap_chain_extent = extent;
    }

    auto Application::createImageViews() -> void
    {
        m_swap_chain_image_views.resize(m_swap_chain_images.size());

        for (std::size_t index{ 0 }; index < m_swap_chain_images.size(); ++index) {
            VkImageViewCreateInfo create_info{};
            create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            create_info.image = m_swap_chain_images[index];
            create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            create_info.format = m_swap_chain_image_format;
            create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            create_info.subresourceRange.baseMipLevel = 0;
            create_info.subresourceRange.levelCount = 1;
            create_info.subresourceRange.baseArrayLayer = 0;
            create_info.subresourceRange.layerCount = 1;

            check(vkCreateImageView(m_device, &create_info, nullptr, &m_swap_chain_image_views[index]), "vkCreateImageView");
        }
    }

    auto Application::createRenderPass() -> void
    {
        VkAttachmentDescription color_attachment{};
        color_attachment.format = m_swap_chain_image_format;
        color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference color_attachment_reference{};
        color_attachment_reference.attachment = 0;
        color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment_reference;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        create_info.attachmentCount = 1;
        create_info.pAttachments = &color_attachment;
        create_info.subpassCount = 1;
        create_info.pSubpasses = &subpass;
        create_info.dependencyCount = 1;
        create_info.pDependencies = &dependency;

        check(vkCreateRenderPass(m_device, &create_info, nullptr, &m_render_pass), "vkCreateRenderPass");
    }

    auto Application::createGraphicsPipeline() -> void
    {
        auto const vert_code{ loadShader("triangle.vert.spv") };
        auto const frag_code{ loadShader("triangle.frag.spv") };
        VkShaderModule const vert_module{ createShaderModule(vert_code) };
        VkShaderModule const frag_module{ createShaderModule(frag_code) };

        VkPipelineShaderStageCreateInfo vert_stage{};
        vert_stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vert_stage.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vert_stage.module = vert_module;
        vert_stage.pName = "vertMain";

        VkPipelineShaderStageCreateInfo frag_stage{};
        frag_stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        frag_stage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        frag_stage.module = frag_module;
        frag_stage.pName = "fragMain";

        std::array<VkPipelineShaderStageCreateInfo, 2> shader_stages{ vert_stage, frag_stage };

        VkPipelineVertexInputStateCreateInfo vertex_input{};
        vertex_input.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        VkPipelineInputAssemblyStateCreateInfo input_assembly{};
        input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        input_assembly.primitiveRestartEnable = VK_FALSE;

        VkPipelineViewportStateCreateInfo viewport_state{};
        viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewport_state.viewportCount = 1;
        viewport_state.scissorCount = 1;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0F;
        rasterizer.cullMode = VK_CULL_MODE_NONE;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState color_blend_attachment{};
        color_blend_attachment.blendEnable = VK_FALSE;
        color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

        VkPipelineColorBlendStateCreateInfo color_blending{};
        color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        color_blending.logicOpEnable = VK_FALSE;
        color_blending.attachmentCount = 1;
        color_blending.pAttachments = &color_blend_attachment;

        std::array<VkDynamicState, 2> dynamic_states{ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
        VkPipelineDynamicStateCreateInfo dynamic_state{};
        dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic_state.dynamicStateCount = static_cast<std::uint32_t>(dynamic_states.size());
        dynamic_state.pDynamicStates = dynamic_states.data();

        VkPipelineLayoutCreateInfo pipeline_layout_info{};
        pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        check(vkCreatePipelineLayout(m_device, &pipeline_layout_info, nullptr, &m_pipeline_layout), "vkCreatePipelineLayout");

        VkGraphicsPipelineCreateInfo pipeline_info{};
        pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipeline_info.stageCount = static_cast<std::uint32_t>(shader_stages.size());
        pipeline_info.pStages = shader_stages.data();
        pipeline_info.pVertexInputState = &vertex_input;
        pipeline_info.pInputAssemblyState = &input_assembly;
        pipeline_info.pViewportState = &viewport_state;
        pipeline_info.pRasterizationState = &rasterizer;
        pipeline_info.pMultisampleState = &multisampling;
        pipeline_info.pColorBlendState = &color_blending;
        pipeline_info.pDynamicState = &dynamic_state;
        pipeline_info.layout = m_pipeline_layout;
        pipeline_info.renderPass = m_render_pass;
        pipeline_info.subpass = 0;

        check(vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &m_graphics_pipeline), "vkCreateGraphicsPipelines");

        vkDestroyShaderModule(m_device, frag_module, nullptr);
        vkDestroyShaderModule(m_device, vert_module, nullptr);
    }

    auto Application::createFramebuffers() -> void
    {
        m_swap_chain_framebuffers.resize(m_swap_chain_image_views.size());

        for (std::size_t index{ 0 }; index < m_swap_chain_image_views.size(); ++index) {
            std::array<VkImageView, 1> attachments{ m_swap_chain_image_views[index] };

            VkFramebufferCreateInfo create_info{};
            create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            create_info.renderPass = m_render_pass;
            create_info.attachmentCount = static_cast<std::uint32_t>(attachments.size());
            create_info.pAttachments = attachments.data();
            create_info.width = m_swap_chain_extent.width;
            create_info.height = m_swap_chain_extent.height;
            create_info.layers = 1;

            check(vkCreateFramebuffer(m_device, &create_info, nullptr, &m_swap_chain_framebuffers[index]), "vkCreateFramebuffer");
        }
    }

    auto Application::createCommandPool() -> void
    {
        VkCommandPoolCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        create_info.queueFamilyIndex = m_graphics_queue_family;

        check(vkCreateCommandPool(m_device, &create_info, nullptr, &m_command_pool), "vkCreateCommandPool");
    }

    auto Application::createCommandBuffers() -> void
    {
        m_command_buffers.resize(MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocate_info{};
        allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocate_info.commandPool = m_command_pool;
        allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocate_info.commandBufferCount = static_cast<std::uint32_t>(m_command_buffers.size());

        check(vkAllocateCommandBuffers(m_device, &allocate_info, m_command_buffers.data()), "vkAllocateCommandBuffers");
    }

    auto Application::createRenderFinishedSemaphores() -> void
    {
        m_render_finished_semaphores.resize(m_swap_chain_images.size());

        VkSemaphoreCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        for (std::size_t index{ 0 }; index < m_render_finished_semaphores.size(); ++index) {
            check(vkCreateSemaphore(m_device, &create_info, nullptr, &m_render_finished_semaphores[index]), "vkCreateSemaphore");
        }
    }

    auto Application::createSyncObjects() -> void
    {
        m_image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphore_info{};
        semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fence_info{};
        fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (std::size_t index{ 0 }; index < MAX_FRAMES_IN_FLIGHT; ++index) {
            check(vkCreateSemaphore(m_device, &semaphore_info, nullptr, &m_image_available_semaphores[index]), "vkCreateSemaphore");
            check(vkCreateFence(m_device, &fence_info, nullptr, &m_in_flight_fences[index]), "vkCreateFence");
        }
    }

    auto Application::drawFrame() -> void
    {
        check(vkWaitForFences(m_device, 1, &m_in_flight_fences[m_current_frame], VK_TRUE, UINT64_MAX), "vkWaitForFences");

        std::uint32_t image_index{ 0 };
        auto const acquire_result{ vkAcquireNextImageKHR(m_device, m_swap_chain, UINT64_MAX, m_image_available_semaphores[m_current_frame], VK_NULL_HANDLE, &image_index) };
        if (acquire_result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        }

        if (acquire_result != VK_SUCCESS && acquire_result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("vkAcquireNextImageKHR failed with VkResult " + std::to_string(static_cast<int>(acquire_result)));
        }

        check(vkResetFences(m_device, 1, &m_in_flight_fences[m_current_frame]), "vkResetFences");
        check(vkResetCommandBuffer(m_command_buffers[m_current_frame], 0), "vkResetCommandBuffer");
        recordCommandBuffer(m_command_buffers[m_current_frame], image_index);

        std::array<VkSemaphore, 1> wait_semaphores{ m_image_available_semaphores[m_current_frame] };
        std::array<VkPipelineStageFlags, 1> wait_stages{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        std::array<VkSemaphore, 1> signal_semaphores{ m_render_finished_semaphores[image_index] };

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.waitSemaphoreCount = static_cast<std::uint32_t>(wait_semaphores.size());
        submit_info.pWaitSemaphores = wait_semaphores.data();
        submit_info.pWaitDstStageMask = wait_stages.data();
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &m_command_buffers[m_current_frame];
        submit_info.signalSemaphoreCount = static_cast<std::uint32_t>(signal_semaphores.size());
        submit_info.pSignalSemaphores = signal_semaphores.data();

        check(vkQueueSubmit(m_graphics_queue, 1, &submit_info, m_in_flight_fences[m_current_frame]), "vkQueueSubmit");

        std::array<VkSwapchainKHR, 1> swap_chains{ m_swap_chain };
        VkPresentInfoKHR present_info{};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.waitSemaphoreCount = static_cast<std::uint32_t>(signal_semaphores.size());
        present_info.pWaitSemaphores = signal_semaphores.data();
        present_info.swapchainCount = static_cast<std::uint32_t>(swap_chains.size());
        present_info.pSwapchains = swap_chains.data();
        present_info.pImageIndices = &image_index;

        auto const present_result{ vkQueuePresentKHR(m_present_queue, &present_info) };
        if (present_result == VK_ERROR_OUT_OF_DATE_KHR || present_result == VK_SUBOPTIMAL_KHR || g_framebuffer_resized) {
            g_framebuffer_resized = false;
            recreateSwapChain();
        } else if (present_result != VK_SUCCESS) {
            throw std::runtime_error("vkQueuePresentKHR failed with VkResult " + std::to_string(static_cast<int>(present_result)));
        }

        m_current_frame = (m_current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    auto Application::recreateSwapChain() -> void
    {
        int width{ 0 };
        int height{ 0 };
        glfwGetFramebufferSize(m_window, &width, &height);
        while ((width == 0 || height == 0) && glfwWindowShouldClose(m_window) == 0) {
            glfwWaitEvents();
            glfwGetFramebufferSize(m_window, &width, &height);
        }
        if (width == 0 || height == 0) {
            return;
        }

        g_framebuffer_resized = false;

        check(vkDeviceWaitIdle(m_device), "vkDeviceWaitIdle");
        cleanupSwapChain();
        createSwapChain();
        createRenderFinishedSemaphores();
        createImageViews();
        createFramebuffers();
    }

    auto Application::cleanupSwapChain() -> void
    {
        for (auto const semaphore : m_render_finished_semaphores) {
            if (semaphore != VK_NULL_HANDLE) {
                vkDestroySemaphore(m_device, semaphore, nullptr);
            }
        }
        m_render_finished_semaphores.clear();

        for (auto const framebuffer : m_swap_chain_framebuffers) {
            if (framebuffer != VK_NULL_HANDLE) {
                vkDestroyFramebuffer(m_device, framebuffer, nullptr);
            }
        }
        m_swap_chain_framebuffers.clear();

        for (auto const image_view : m_swap_chain_image_views) {
            if (image_view != VK_NULL_HANDLE) {
                vkDestroyImageView(m_device, image_view, nullptr);
            }
        }
        m_swap_chain_image_views.clear();

        if (m_swap_chain != VK_NULL_HANDLE) {
            vkDestroySwapchainKHR(m_device, m_swap_chain, nullptr);
            m_swap_chain = VK_NULL_HANDLE;
        }
    }

    auto Application::loadShader(char const* file_name) const -> std::vector<char>
    {
        return read_file(m_shader_dir / file_name);
    }

    auto Application::checkValidationLayerSupport() const -> bool
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

    auto Application::getRequiredExtensions() const -> std::vector<char const*>
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

    auto Application::findQueueFamilies(VkPhysicalDevice device) const -> QueueFamilyIndices
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
            check(vkGetPhysicalDeviceSurfaceSupportKHR(device, index, m_surface, &present_support), "vkGetPhysicalDeviceSurfaceSupportKHR");
            if (present_support == VK_TRUE) {
                indices.present_family = index;
            }

            if (indices.isComplete()) {
                break;
            }
        }

        return indices;
    }

    auto Application::querySwapChainSupport(VkPhysicalDevice device) const -> SwapChainSupportDetails
    {
        SwapChainSupportDetails details;

        check(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities), "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");

        std::uint32_t format_count{ 0 };
        check(vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &format_count, nullptr), "vkGetPhysicalDeviceSurfaceFormatsKHR");
        if (format_count != 0) {
            details.formats.resize(format_count);
            check(vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &format_count, details.formats.data()), "vkGetPhysicalDeviceSurfaceFormatsKHR");
        }

        std::uint32_t present_mode_count{ 0 };
        check(vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &present_mode_count, nullptr), "vkGetPhysicalDeviceSurfacePresentModesKHR");
        if (present_mode_count != 0) {
            details.present_modes.resize(present_mode_count);
            check(vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &present_mode_count, details.present_modes.data()), "vkGetPhysicalDeviceSurfacePresentModesKHR");
        }

        return details;
    }

    auto Application::checkDeviceExtensionSupport(VkPhysicalDevice device) const -> bool
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

    auto Application::isDeviceSuitable(VkPhysicalDevice device) const -> bool
    {
        auto const indices{ findQueueFamilies(device) };
        if (!indices.isComplete() || !checkDeviceExtensionSupport(device)) {
            return false;
        }

        auto const support{ querySwapChainSupport(device) };
        return !support.formats.empty() && !support.present_modes.empty();
    }

    auto Application::chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> const& formats) const -> VkSurfaceFormatKHR
    {
        for (auto const& format : formats) {
            if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return format;
            }
        }

        return formats.front();
    }

    auto Application::chooseSwapPresentMode(std::vector<VkPresentModeKHR> const& present_modes) const -> VkPresentModeKHR
    {
        for (auto const mode : present_modes) {
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return mode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    auto Application::chooseSwapExtent(VkSurfaceCapabilitiesKHR const& capabilities) const -> VkExtent2D
    {
        if (capabilities.currentExtent.width != std::numeric_limits<std::uint32_t>::max()) {
            return capabilities.currentExtent;
        }

        int width{ 0 };
        int height{ 0 };
        glfwGetFramebufferSize(m_window, &width, &height);

        VkExtent2D actual_extent{
            static_cast<std::uint32_t>(width),
            static_cast<std::uint32_t>(height)
        };
        actual_extent.width = std::clamp(actual_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actual_extent.height = std::clamp(actual_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        return actual_extent;
    }

    auto Application::createShaderModule(std::vector<char> const& code) const -> VkShaderModule
    {
        VkShaderModuleCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        create_info.codeSize = code.size();
        create_info.pCode = reinterpret_cast<std::uint32_t const*>(code.data());

        VkShaderModule module{ VK_NULL_HANDLE };
        check(vkCreateShaderModule(m_device, &create_info, nullptr, &module), "vkCreateShaderModule");
        return module;
    }

    auto Application::recordCommandBuffer(VkCommandBuffer command_buffer, std::uint32_t image_index) const -> void
    {
        VkCommandBufferBeginInfo begin_info{};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        check(vkBeginCommandBuffer(command_buffer, &begin_info), "vkBeginCommandBuffer");

        std::array<VkClearValue, 1> clear_values{};
        clear_values[0].color.float32[0] = 0.05F;
        clear_values[0].color.float32[1] = 0.05F;
        clear_values[0].color.float32[2] = 0.08F;
        clear_values[0].color.float32[3] = 1.0F;

        VkRenderPassBeginInfo render_pass_info{};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_info.renderPass = m_render_pass;
        render_pass_info.framebuffer = m_swap_chain_framebuffers[image_index];
        render_pass_info.renderArea.offset = { 0, 0 };
        render_pass_info.renderArea.extent = m_swap_chain_extent;
        render_pass_info.clearValueCount = static_cast<std::uint32_t>(clear_values.size());
        render_pass_info.pClearValues = clear_values.data();

        vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphics_pipeline);

        VkViewport viewport{};
        viewport.x = 0.0F;
        viewport.y = 0.0F;
        viewport.width = static_cast<float>(m_swap_chain_extent.width);
        viewport.height = static_cast<float>(m_swap_chain_extent.height);
        viewport.minDepth = 0.0F;
        viewport.maxDepth = 1.0F;
        vkCmdSetViewport(command_buffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = m_swap_chain_extent;
        vkCmdSetScissor(command_buffer, 0, 1, &scissor);

        vkCmdDraw(command_buffer, 3, 1, 0, 0);
        vkCmdEndRenderPass(command_buffer);

        check(vkEndCommandBuffer(command_buffer), "vkEndCommandBuffer");
    }

} // namespace

auto main() -> int
{
    try {
        Application application{ executable_directory() / "shaders" };
        return application.Run();
    } catch (std::exception const& exception) {
        std::cerr << "Fatal error: " << exception.what() << '\n';
        return EXIT_FAILURE;
    }
}
