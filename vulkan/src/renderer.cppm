module;

#include <vulkan/vulkan.h>

module vulkan:renderer;

import std;

import :common;
import :device;
import :swap_chain;
import :pipeline;

namespace vulkan {

    enum class DrawResult {
        Success,
        RecreateSwapChain,
    };

    class Renderer {
    public:
        Renderer(VkDevice device, std::uint32_t graphics_queue_family);
        ~Renderer();

        Renderer(Renderer const&) = delete;
        Renderer& operator=(Renderer const&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(Renderer&&) = delete;

        DrawResult DrawFrame(
            VulkanContext const& context,
            SwapChain const& swap_chain,
            GraphicsPipeline const& pipeline
        );

    private:
        VkDevice m_device{ VK_NULL_HANDLE };
        VkCommandPool m_command_pool{ VK_NULL_HANDLE };
        std::vector<VkCommandBuffer> m_command_buffers;
        std::vector<VkSemaphore> m_image_available_semaphores;
        std::vector<VkFence> m_in_flight_fences;
        std::uint32_t m_current_frame{ 0 };

        void createCommandPool(std::uint32_t graphics_queue_family);
        void createCommandBuffers();
        void createSyncObjects();
        void recordCommandBuffer(
            VkCommandBuffer command_buffer,
            std::uint32_t image_index,
            SwapChain const& swap_chain,
            GraphicsPipeline const& pipeline
        ) const;
    };

    Renderer::Renderer(VkDevice device, std::uint32_t graphics_queue_family) : m_device{ device }
    {
        createCommandPool(graphics_queue_family);
        createCommandBuffers();
        createSyncObjects();
    }

    Renderer::~Renderer()
    {
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
    }

    DrawResult Renderer::DrawFrame(
        VulkanContext const& context,
        SwapChain const& swap_chain,
        GraphicsPipeline const& pipeline
    )
    {
        check(vkWaitForFences(m_device, 1, &m_in_flight_fences[m_current_frame], VK_TRUE, UINT64_MAX), "vkWaitForFences");

        std::uint32_t image_index{ 0 };
        auto const acquire_result{ vkAcquireNextImageKHR(m_device, swap_chain.Handle(), UINT64_MAX, m_image_available_semaphores[m_current_frame], VK_NULL_HANDLE, &image_index) };
        if (acquire_result == VK_ERROR_OUT_OF_DATE_KHR) {
            return DrawResult::RecreateSwapChain;
        }

        if (acquire_result != VK_SUCCESS && acquire_result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("vkAcquireNextImageKHR failed with VkResult " + std::to_string(static_cast<int>(acquire_result)));
        }

        check(vkResetFences(m_device, 1, &m_in_flight_fences[m_current_frame]), "vkResetFences");
        check(vkResetCommandBuffer(m_command_buffers[m_current_frame], 0), "vkResetCommandBuffer");
        recordCommandBuffer(m_command_buffers[m_current_frame], image_index, swap_chain, pipeline);

        std::array<VkSemaphore, 1> wait_semaphores{ m_image_available_semaphores[m_current_frame] };
        std::array<VkPipelineStageFlags, 1> wait_stages{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        std::array<VkSemaphore, 1> signal_semaphores{ swap_chain.RenderFinishedSemaphore(image_index) };

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.waitSemaphoreCount = static_cast<std::uint32_t>(wait_semaphores.size());
        submit_info.pWaitSemaphores = wait_semaphores.data();
        submit_info.pWaitDstStageMask = wait_stages.data();
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &m_command_buffers[m_current_frame];
        submit_info.signalSemaphoreCount = static_cast<std::uint32_t>(signal_semaphores.size());
        submit_info.pSignalSemaphores = signal_semaphores.data();

        check(vkQueueSubmit(context.GraphicsQueue(), 1, &submit_info, m_in_flight_fences[m_current_frame]), "vkQueueSubmit");

        std::array<VkSwapchainKHR, 1> swap_chains{ swap_chain.Handle() };
        VkPresentInfoKHR present_info{};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.waitSemaphoreCount = static_cast<std::uint32_t>(signal_semaphores.size());
        present_info.pWaitSemaphores = signal_semaphores.data();
        present_info.swapchainCount = static_cast<std::uint32_t>(swap_chains.size());
        present_info.pSwapchains = swap_chains.data();
        present_info.pImageIndices = &image_index;

        auto const present_result{ vkQueuePresentKHR(context.PresentQueue(), &present_info) };
        bool const recreate_swap_chain{ present_result == VK_ERROR_OUT_OF_DATE_KHR || present_result == VK_SUBOPTIMAL_KHR };
        if (!recreate_swap_chain && present_result != VK_SUCCESS) {
            throw std::runtime_error("vkQueuePresentKHR failed with VkResult " + std::to_string(static_cast<int>(present_result)));
        }

        m_current_frame = (m_current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
        return recreate_swap_chain ? DrawResult::RecreateSwapChain : DrawResult::Success;
    }

    void Renderer::createCommandPool(std::uint32_t graphics_queue_family)
    {
        VkCommandPoolCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        create_info.queueFamilyIndex = graphics_queue_family;

        check(vkCreateCommandPool(m_device, &create_info, nullptr, &m_command_pool), "vkCreateCommandPool");
    }

    void Renderer::createCommandBuffers()
    {
        m_command_buffers.resize(MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocate_info{};
        allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocate_info.commandPool = m_command_pool;
        allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocate_info.commandBufferCount = static_cast<std::uint32_t>(m_command_buffers.size());

        check(vkAllocateCommandBuffers(m_device, &allocate_info, m_command_buffers.data()), "vkAllocateCommandBuffers");
    }

    void Renderer::createSyncObjects()
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

    void Renderer::recordCommandBuffer(
        VkCommandBuffer command_buffer,
        std::uint32_t image_index,
        SwapChain const& swap_chain,
        GraphicsPipeline const& pipeline
    ) const
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
        render_pass_info.renderPass = pipeline.RenderPass();
        render_pass_info.framebuffer = swap_chain.Framebuffer(image_index);
        render_pass_info.renderArea.offset = { 0, 0 };
        render_pass_info.renderArea.extent = swap_chain.Extent();
        render_pass_info.clearValueCount = static_cast<std::uint32_t>(clear_values.size());
        render_pass_info.pClearValues = clear_values.data();

        vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.Handle());

        VkViewport viewport{};
        viewport.x = 0.0F;
        viewport.y = 0.0F;
        viewport.width = static_cast<float>(swap_chain.Extent().width);
        viewport.height = static_cast<float>(swap_chain.Extent().height);
        viewport.minDepth = 0.0F;
        viewport.maxDepth = 1.0F;
        vkCmdSetViewport(command_buffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = swap_chain.Extent();
        vkCmdSetScissor(command_buffer, 0, 1, &scissor);

        vkCmdDraw(command_buffer, 3, 1, 0, 0);
        vkCmdEndRenderPass(command_buffer);

        check(vkEndCommandBuffer(command_buffer), "vkEndCommandBuffer");
    }

} // namespace vulkan
