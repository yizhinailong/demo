module;

#include <vulkan/vulkan.h>

module vulkan:pipeline;

import std;

import :common;

namespace vulkan {

    class GraphicsPipeline {
    public:
        GraphicsPipeline(
            VkDevice device,
            VkFormat color_format,
            std::filesystem::path const& shader_dir
        );
        ~GraphicsPipeline();

        GraphicsPipeline(GraphicsPipeline const&) = delete;
        GraphicsPipeline& operator=(GraphicsPipeline const&) = delete;
        GraphicsPipeline(GraphicsPipeline&&) = delete;
        GraphicsPipeline& operator=(GraphicsPipeline&&) = delete;

        [[nodiscard]] VkRenderPass RenderPass() const noexcept;
        [[nodiscard]] VkPipeline Handle() const noexcept;

    private:
        VkDevice m_device{ VK_NULL_HANDLE };
        VkRenderPass m_render_pass{ VK_NULL_HANDLE };
        VkPipelineLayout m_pipeline_layout{ VK_NULL_HANDLE };
        VkPipeline m_graphics_pipeline{ VK_NULL_HANDLE };

        void createRenderPass(VkFormat color_format);
        void createPipeline(std::filesystem::path const& shader_dir);
        [[nodiscard]] VkShaderModule createShaderModule(std::vector<char> const& code) const;
        [[nodiscard]] std::vector<char> loadShader(
            std::filesystem::path const& shader_dir,
            char const* file_name
        ) const;
    };

    GraphicsPipeline::GraphicsPipeline(
        VkDevice device,
        VkFormat color_format,
        std::filesystem::path const& shader_dir
    ) : m_device{ device }
    {
        createRenderPass(color_format);
        createPipeline(shader_dir);
    }

    GraphicsPipeline::~GraphicsPipeline()
    {
        if (m_graphics_pipeline != VK_NULL_HANDLE) {
            vkDestroyPipeline(m_device, m_graphics_pipeline, nullptr);
        }

        if (m_pipeline_layout != VK_NULL_HANDLE) {
            vkDestroyPipelineLayout(m_device, m_pipeline_layout, nullptr);
        }

        if (m_render_pass != VK_NULL_HANDLE) {
            vkDestroyRenderPass(m_device, m_render_pass, nullptr);
        }
    }

    VkRenderPass GraphicsPipeline::RenderPass() const noexcept
    {
        return m_render_pass;
    }

    VkPipeline GraphicsPipeline::Handle() const noexcept
    {
        return m_graphics_pipeline;
    }

    void GraphicsPipeline::createRenderPass(VkFormat color_format)
    {
        VkAttachmentDescription color_attachment{};
        color_attachment.format = color_format;
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

    void GraphicsPipeline::createPipeline(std::filesystem::path const& shader_dir)
    {
        auto const vert_code{ loadShader(shader_dir, "triangle.vert.spv") };
        auto const frag_code{ loadShader(shader_dir, "triangle.frag.spv") };
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

    VkShaderModule GraphicsPipeline::createShaderModule(std::vector<char> const& code) const
    {
        VkShaderModuleCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        create_info.codeSize = code.size();
        create_info.pCode = reinterpret_cast<std::uint32_t const*>(code.data());

        VkShaderModule module{ VK_NULL_HANDLE };
        check(vkCreateShaderModule(m_device, &create_info, nullptr, &module), "vkCreateShaderModule");
        return module;
    }

    std::vector<char> GraphicsPipeline::loadShader(
        std::filesystem::path const& shader_dir,
        char const* file_name
    ) const
    {
        return read_file(shader_dir / file_name);
    }

} // namespace vulkan
