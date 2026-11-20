module;

#include <vulkan/vulkan.h>

module vulkan:swap_chain;

import std;

import :common;

namespace vulkan {

    class SwapChain {
    public:
        SwapChain(
            VkPhysicalDevice physical_device,
            VkDevice device,
            VkSurfaceKHR surface,
            std::uint32_t graphics_queue_family,
            std::uint32_t present_queue_family,
            std::uint32_t width,
            std::uint32_t height
        );
        ~SwapChain();

        SwapChain(SwapChain const&) = delete;
        SwapChain& operator=(SwapChain const&) = delete;
        SwapChain(SwapChain&&) = delete;
        SwapChain& operator=(SwapChain&&) = delete;

        [[nodiscard]] VkSwapchainKHR Handle() const noexcept;
        [[nodiscard]] VkFormat ImageFormat() const noexcept;
        [[nodiscard]] VkExtent2D Extent() const noexcept;
        [[nodiscard]] VkFramebuffer Framebuffer(std::uint32_t image_index) const;
        [[nodiscard]] VkSemaphore RenderFinishedSemaphore(std::uint32_t image_index) const;

        void CreateFramebuffers(VkRenderPass render_pass);
        void Recreate(std::uint32_t width, std::uint32_t height, VkRenderPass render_pass);

    private:
        VkPhysicalDevice m_physical_device{ VK_NULL_HANDLE };
        VkDevice m_device{ VK_NULL_HANDLE };
        VkSurfaceKHR m_surface{ VK_NULL_HANDLE };
        std::uint32_t m_graphics_queue_family{ 0 };
        std::uint32_t m_present_queue_family{ 0 };

        VkSwapchainKHR m_swap_chain{ VK_NULL_HANDLE };
        std::vector<VkImage> m_images;
        VkFormat m_image_format{ VK_FORMAT_UNDEFINED };
        VkExtent2D m_extent{};
        std::vector<VkImageView> m_image_views;
        std::vector<VkFramebuffer> m_framebuffers;
        std::vector<VkSemaphore> m_render_finished_semaphores;

        void createSwapChain(std::uint32_t width, std::uint32_t height);
        void createImageViews();
        void createRenderFinishedSemaphores();
        void cleanup();
    };

    SwapChain::SwapChain(
        VkPhysicalDevice physical_device,
        VkDevice device,
        VkSurfaceKHR surface,
        std::uint32_t graphics_queue_family,
        std::uint32_t present_queue_family,
        std::uint32_t width,
        std::uint32_t height
    ) : m_physical_device{ physical_device },
        m_device{ device },
        m_surface{ surface },
        m_graphics_queue_family{ graphics_queue_family },
        m_present_queue_family{ present_queue_family }
    {
        createSwapChain(width, height);
        createRenderFinishedSemaphores();
        createImageViews();
    }

    SwapChain::~SwapChain()
    {
        cleanup();
    }

    VkSwapchainKHR SwapChain::Handle() const noexcept
    {
        return m_swap_chain;
    }

    VkFormat SwapChain::ImageFormat() const noexcept
    {
        return m_image_format;
    }

    VkExtent2D SwapChain::Extent() const noexcept
    {
        return m_extent;
    }

    VkFramebuffer SwapChain::Framebuffer(std::uint32_t image_index) const
    {
        return m_framebuffers[image_index];
    }

    VkSemaphore SwapChain::RenderFinishedSemaphore(std::uint32_t image_index) const
    {
        return m_render_finished_semaphores[image_index];
    }

    void SwapChain::CreateFramebuffers(VkRenderPass render_pass)
    {
        m_framebuffers.resize(m_image_views.size());

        for (std::size_t index{ 0 }; index < m_image_views.size(); ++index) {
            std::array<VkImageView, 1> attachments{ m_image_views[index] };

            VkFramebufferCreateInfo create_info{};
            create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            create_info.renderPass = render_pass;
            create_info.attachmentCount = static_cast<std::uint32_t>(attachments.size());
            create_info.pAttachments = attachments.data();
            create_info.width = m_extent.width;
            create_info.height = m_extent.height;
            create_info.layers = 1;

            check(vkCreateFramebuffer(m_device, &create_info, nullptr, &m_framebuffers[index]), "vkCreateFramebuffer");
        }
    }

    void SwapChain::Recreate(std::uint32_t width, std::uint32_t height, VkRenderPass render_pass)
    {
        cleanup();
        createSwapChain(width, height);
        createRenderFinishedSemaphores();
        createImageViews();
        CreateFramebuffers(render_pass);
    }

    void SwapChain::createSwapChain(std::uint32_t width, std::uint32_t height)
    {
        auto const support{ query_swap_chain_support(m_physical_device, m_surface) };
        auto const surface_format{ choose_swap_surface_format(support.formats) };
        auto const present_mode{ choose_swap_present_mode(support.present_modes) };
        auto const extent{ choose_swap_extent(support.capabilities, width, height) };

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
        m_images.resize(swap_chain_image_count);
        check(vkGetSwapchainImagesKHR(m_device, m_swap_chain, &swap_chain_image_count, m_images.data()), "vkGetSwapchainImagesKHR");

        m_image_format = surface_format.format;
        m_extent = extent;
    }

    void SwapChain::createImageViews()
    {
        m_image_views.resize(m_images.size());

        for (std::size_t index{ 0 }; index < m_images.size(); ++index) {
            VkImageViewCreateInfo create_info{};
            create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            create_info.image = m_images[index];
            create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            create_info.format = m_image_format;
            create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            create_info.subresourceRange.baseMipLevel = 0;
            create_info.subresourceRange.levelCount = 1;
            create_info.subresourceRange.baseArrayLayer = 0;
            create_info.subresourceRange.layerCount = 1;

            check(vkCreateImageView(m_device, &create_info, nullptr, &m_image_views[index]), "vkCreateImageView");
        }
    }

    void SwapChain::createRenderFinishedSemaphores()
    {
        m_render_finished_semaphores.resize(m_images.size());

        VkSemaphoreCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        for (std::size_t index{ 0 }; index < m_render_finished_semaphores.size(); ++index) {
            check(vkCreateSemaphore(m_device, &create_info, nullptr, &m_render_finished_semaphores[index]), "vkCreateSemaphore");
        }
    }

    void SwapChain::cleanup()
    {
        for (auto const semaphore : m_render_finished_semaphores) {
            if (semaphore != VK_NULL_HANDLE) {
                vkDestroySemaphore(m_device, semaphore, nullptr);
            }
        }
        m_render_finished_semaphores.clear();

        for (auto const framebuffer : m_framebuffers) {
            if (framebuffer != VK_NULL_HANDLE) {
                vkDestroyFramebuffer(m_device, framebuffer, nullptr);
            }
        }
        m_framebuffers.clear();

        for (auto const image_view : m_image_views) {
            if (image_view != VK_NULL_HANDLE) {
                vkDestroyImageView(m_device, image_view, nullptr);
            }
        }
        m_image_views.clear();

        if (m_swap_chain != VK_NULL_HANDLE) {
            vkDestroySwapchainKHR(m_device, m_swap_chain, nullptr);
            m_swap_chain = VK_NULL_HANDLE;
        }

        m_images.clear();
    }

} // namespace vulkan
