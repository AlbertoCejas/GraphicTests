#include <PotatoApp/PotatoAppSample.h>

#include <cstdint>
#include <PotatoEngine/Core/PluginManager.h>
#include <PotatoEngine/Core/RendererPlugin.h>
#include <PotatoEngine/File/File.h>
#include <PotatoEngine/File/FileSystem.h>
#include <PotatoEngine/Render/interface/IPipelineState.h>
#include <PotatoEngine/Render/interface/IRenderDevice.h>
#include <PotatoEngine/Render/interface/IRenderPass.h>
#include <PotatoEngine/Render/interface/IShader.h>
#include <PotatoEngine/Util/Assert.h>

using namespace potato;

void PotatoAppSample::initImpl(const nlohmann::json& configJson)
{
    const auto plugins = m_engine.getPluginManager()->getActivePlugins();
    for (auto plugin : plugins)
    {
        if (plugin->getType() == PluginType::Renderer)
        {
            m_renderer = static_cast<RendererPlugin*>(plugin);
        }
    }
    POTATO_ASSERT_MSG(m_renderer != nullptr, "No renderer plugin could be found");

    createRenderPass();
}

void PotatoAppSample::shutdown()
{
    
    Application::shutdown();
}

void PotatoAppSample::updateImpl()
{

}

void PotatoAppSample::createRenderPass()
{
    // Attachment 0 - Color buffer
    // Attachment 1 - Depth buffer
    // Attachment 3 - Color resolve buffer
    constexpr uint32_t numAttachments = 3;

    // Prepare render pass attachment descriptions
    RenderPassAttachmentDesc attachments[numAttachments];
    attachments[0].format = TextureFormat::TEX_FORMAT_BGRA8_UNORM_SRGB;
    attachments[0].initialState = ResourceState::RESOURCE_STATE_UNDEFINED;
    attachments[0].finalState = ResourceState::RESOURCE_STATE_RENDER_TARGET;
    attachments[0].loadOp = AttachmentLoadOp::ATTACHMENT_LOAD_OP_CLEAR;
    attachments[0].storeOp = AttachmentStoreOp::ATTACHMENT_STORE_OP_STORE;
    attachments[0].stencilLoadOp = AttachmentLoadOp::ATTACHMENT_LOAD_OP_DISCARD;
    attachments[0].stencilStoreOp = AttachmentStoreOp::ATTACHMENT_STORE_OP_DISCARD;
    attachments[0].sampleCount = 4;

    attachments[1].format = TextureFormat::TEX_FORMAT_D32_FLOAT;
    attachments[1].initialState = ResourceState::RESOURCE_STATE_UNKNOWN;
    attachments[1].finalState = ResourceState::RESOURCE_STATE_DEPTH_WRITE;
    attachments[1].loadOp = AttachmentLoadOp::ATTACHMENT_LOAD_OP_CLEAR;
    attachments[1].storeOp = AttachmentStoreOp::ATTACHMENT_STORE_OP_DISCARD;
    attachments[1].stencilLoadOp = AttachmentLoadOp::ATTACHMENT_LOAD_OP_DISCARD;
    attachments[1].stencilStoreOp = AttachmentStoreOp::ATTACHMENT_STORE_OP_DISCARD;
    attachments[1].sampleCount = 4;

    attachments[2].format = TextureFormat::TEX_FORMAT_BGRA8_UNORM_SRGB;
    attachments[2].initialState = ResourceState::RESOURCE_STATE_UNKNOWN;
    attachments[2].finalState = ResourceState::RESOURCE_STATE_PRESENT;
    attachments[2].loadOp = AttachmentLoadOp::ATTACHMENT_LOAD_OP_DISCARD;
    attachments[2].storeOp = AttachmentStoreOp::ATTACHMENT_STORE_OP_STORE;
    attachments[2].stencilLoadOp = AttachmentLoadOp::ATTACHMENT_LOAD_OP_DISCARD;
    attachments[2].stencilStoreOp = AttachmentStoreOp::ATTACHMENT_STORE_OP_DISCARD;
    attachments[2].sampleCount = 1;


    // Subpass 1 - Lighting
    constexpr uint32_t numSubpasses = 1;

    // Prepare subpass descriptions
    SubpassDesc subpasses[numSubpasses];

    // Subpass 1 attachments - 1, render target
    subpasses[0].renderTargetAttachments = { { 0, ResourceState::RESOURCE_STATE_RENDER_TARGET } };

    // Subpass 1 attachments - 2, depth buffer
    AttachmentReference depthAttachmentRef = { 1, ResourceState::RESOURCE_STATE_DEPTH_WRITE };
    subpasses[0].depthStencilAttachment = &depthAttachmentRef;

    // Subpass 1 attachments - 3, present render target
    subpasses[0].resolveAttachments = { { 2, ResourceState::RESOURCE_STATE_RENDER_TARGET } };

    constexpr uint32_t numDependencies = 1;

    SubpassDependencyDesc dependencies[numDependencies];
    dependencies[0].srcSubpass = SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = PipelineStageFlags::PIPELINE_STAGE_FLAG_RENDER_TARGET;
    dependencies[0].dstStageMask = PipelineStageFlags::PIPELINE_STAGE_FLAG_RENDER_TARGET;
    dependencies[0].srcAccessMask = AccessFlags::ACCESS_FLAG_NONE;
    dependencies[0].dstAccessMask = AccessFlags::ACCESS_FLAG_RENDER_TARGET_WRITE;

    RenderPassDesc renderPassDesc =
    {
        .name = "Lighting",
        .attachments = std::vector<const RenderPassAttachmentDesc>(attachments, attachments + numAttachments),
        .subpasses = std::vector<const SubpassDesc>(subpasses, subpasses + numSubpasses),
        .dependencies = std::vector<const SubpassDependencyDesc>(dependencies, dependencies + numDependencies)
    };

    m_renderPass = m_renderer->getRenderDevice().createRenderPass(renderPassDesc);
    POTATO_ASSERT_MSG(m_renderPass != nullptr, "render pass could not be created");
}


void PotatoAppSample::createGraphicsPipeline()
{
    IRenderDevice& renderDevice = m_renderer->getRenderDevice();

    GraphicsPipelineStateCreateInfo PSOCreateInfo;
    PipelineStateDesc& PSODesc = PSOCreateInfo.PSODesc;

    // Pipeline state name is used by the engine to report issues.
    PSODesc.name = "PSO";

    PSOCreateInfo.graphicsPipeline.renderPass = m_renderPass;
    //PSOCreateInfo.GraphicsPipeline.SubpassIndex = 0; // This PSO will be used within the first subpass
    // When m_renderPass is not null, all RTVFormats and DSVFormat must be TEX_FORMAT_UNKNOWN,
    // while NumRenderTargets must be 0

    //PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    //PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
    //PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = True;

    ShaderCreateInfo shaderCI;

    // Tell the system that the shader source code is in HLSL.
    // For OpenGL, the engine will convert this into GLSL under the hood.
    //ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;

    // OpenGL backend requires emulated combined HLSL texture samplers (g_Texture + g_Texture_sampler combination)
    //ShaderCI.UseCombinedTextureSamplers = true;

    //ShaderCI.pShaderSourceStreamFactory = pShaderSourceFactory;

    // Create default vertex shader

    IShader* vsPtr;
    {
        auto vertexShaderFilePtr = FileSystem::openFile("../../Data/shaders/vert.spv", FileAccessMode::Read);
        const size_t fileSize = vertexShaderFilePtr->getSize();
        char* vertexShaderBuffer = (char*)malloc(fileSize);
        vertexShaderFilePtr->read(vertexShaderBuffer, fileSize);

	    shaderCI.desc.shaderType = SHADER_TYPE_VERTEX;
	    //shaderCI.entryPoint = "main";
	    shaderCI.desc.name = "Default VS";
	    shaderCI.byteCode = vertexShaderBuffer;
        shaderCI.byteCodeSize = fileSize;
	    vsPtr = renderDevice.createShader(shaderCI);
	    POTATO_ASSERT(vsPtr != nullptr);

        free(vertexShaderBuffer);
    }

    IShader* fsPtr;
    {
        auto fragmentShaderFilePtr = FileSystem::openFile("../../Data/shaders/frag.spv", FileAccessMode::Read);
        const size_t fileSize = fragmentShaderFilePtr->getSize();
        char* fragmentShaderBuffer = (char*)malloc(fileSize);
        fragmentShaderFilePtr->read(fragmentShaderBuffer, fileSize);

        shaderCI.desc.shaderType = SHADER_TYPE_PIXEL;
        //shaderCI.entryPoint = "main";
        shaderCI.desc.name = "Default FS";
        shaderCI.byteCode = fragmentShaderBuffer;
        shaderCI.byteCodeSize = fileSize;
        fsPtr = renderDevice.createShader(shaderCI);
        POTATO_ASSERT(vsPtr != nullptr);

        free(fragmentShaderBuffer);
    }

    /*const LayoutElement layoutElems[] =
    {
        LayoutElement{ .inputIndex = 0, .bufferSlot = 0, .numComponents = 3, .valueType = VT_FLOAT32, .isNormalized = false}, // Attribute 0 - vertex position
        LayoutElement{ .inputIndex = 1, .bufferSlot = 0, .numComponents = 2, .valueType = VT_FLOAT32, .isNormalized = false}  // Attribute 1 - texture coordinates
    };*/

    PSOCreateInfo.vsPtr = vsPtr;
    PSOCreateInfo.fsPtr = fsPtr;

    //PSOCreateInfo.graphicsPipeline.inputLayout.layoutElements = layoutElems;
    //PSOCreateInfo.graphicsPipeline.inputLayout.numElements    = std::size(layoutElems);

    // Define variable type that will be used by default
    /*PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

    // clang-format off
    ShaderResourceVariableDesc Vars[] = 
    {
        {SHADER_TYPE_PIXEL, "g_Texture", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE}
    };
    // clang-format on
    PSODesc.ResourceLayout.Variables    = Vars;
    PSODesc.ResourceLayout.NumVariables = _countof(Vars);

    // clang-format off
    // Define immutable sampler for g_Texture.
    SamplerDesc SamLinearClampDesc
    {
        FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, 
        TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP
    };
    ImmutableSamplerDesc ImtblSamplers[] = 
    {
        {SHADER_TYPE_PIXEL, "g_Texture", SamLinearClampDesc}
    };
    // clang-format on
    PSODesc.ResourceLayout.ImmutableSamplers    = ImtblSamplers;
    PSODesc.ResourceLayout.NumImmutableSamplers = _countof(ImtblSamplers);*/

    m_pipelineState = renderDevice.createGraphicsPipelineState(PSOCreateInfo);


    /*
    
    auto vertShaderCode = readFile("../../Data/shaders/vert.spv");
    auto fragShaderCode = readFile("../../Data/shaders/frag.spv");

    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);
    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

    // Vertex buffer
    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    // Viewport
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)m_swapChainExtent.width;
    viewport.height = (float)m_swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = m_swapChainExtent;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    // Rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f; // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

    // Multisampling
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_TRUE; // enable sample shading in the pipeline
    multisampling.minSampleShading = .2f; // min fraction for sample shading; closer to one is smoother
    multisampling.rasterizationSamples = m_msaaSamples;
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional

    // Color Blending
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional

    // Depth
    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f; // Optional
    depthStencil.maxDepthBounds = 1.0f; // Optional
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.front = {}; // Optional
    depthStencil.back = {}; // Optional


    // Pipeline layout
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1; // Optional
    pipelineLayoutInfo.pSetLayouts = &m_descriptorSetLayout; // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

    if (vkCreatePipelineLayout(m_logicalDevice, &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    // Graphics pipeline
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr; // Optional
    pipelineInfo.layout = m_pipelineLayout;
    pipelineInfo.renderPass = m_renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optional


    if (vkCreateGraphicsPipelines(m_logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    vkDestroyShaderModule(m_logicalDevice, fragShaderModule, nullptr);
    vkDestroyShaderModule(m_logicalDevice, vertShaderModule, nullptr);

    */
}