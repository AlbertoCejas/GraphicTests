#include <Core/include/VKPipelineState.h>

#include <Core/include/VKObjectWrapper.h>
#include <Core/include/VKLogicalDevice.h>
#include <Core/include/VKShader.h>
#include <Core/include/Utils/VKConversions.h>


using namespace potato;
using namespace potato::vk;


void createGraphicsPipeline(VKRenderDevice& device,
    std::vector<VkPipelineShaderStageCreateInfo>& stages,
    const PipelineLayout& layout,
    const PipelineStateDesc& PSODesc,
    const GraphicsPipelineDesc& graphicsPipeline,
    PipelineWrapper& pipeline,
    IRenderPass& renderPass)
{
    const auto& logicalDevice = device.getVKLogicalDevice();
    const auto& physicalDevice = device.getVKPhysicalDevice();
    /*auto& RPCache = device->GetImplicitRenderPassCache();

    if (pRenderPass == nullptr)
    {
        RenderPassCache::RenderPassCacheKey Key{
            GraphicsPipeline.NumRenderTargets,
            GraphicsPipeline.SmplDesc.Count,
            GraphicsPipeline.RTVFormats,
            GraphicsPipeline.DSVFormat };
        pRenderPass = RPCache.GetRenderPass(Key);
    }*/

    VkGraphicsPipelineCreateInfo pipelineCI = {};

    pipelineCI.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCI.pNext = nullptr;
    pipelineCI.stageCount = static_cast<uint32_t>(stages.size());
    pipelineCI.pStages = stages.data();
    pipelineCI.layout = layout.getVkPipelineLayout();

    VkPipelineVertexInputStateCreateInfo vertexInputStateCI = {};

    std::array<VkVertexInputBindingDescription, MAX_LAYOUT_ELEMENTS>   bindingDescriptions;
    std::array<VkVertexInputAttributeDescription, MAX_LAYOUT_ELEMENTS> attributeDescription;
    inputLayoutDescToVkVertexInputStateCI(graphicsPipeline.inputLayout, vertexInputStateCI, bindingDescriptions, attributeDescription);
    pipelineCI.pVertexInputState = &vertexInputStateCI;

    /*
    VkPipelineInputAssemblyStateCreateInfo InputAssemblyCI = {};

    InputAssemblyCI.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    InputAssemblyCI.pNext = nullptr;
    InputAssemblyCI.flags = 0; // reserved for future use
    InputAssemblyCI.primitiveRestartEnable = VK_FALSE;
    pipelineCI.pInputAssemblyState = &InputAssemblyCI;


    VkPipelineTessellationStateCreateInfo TessStateCI = {};

    TessStateCI.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    TessStateCI.pNext = nullptr;
    TessStateCI.flags = 0; // reserved for future use
    pipelineCI.pTessellationState = &TessStateCI;

    if (PSODesc.pipelineType == PIPELINE_TYPE_MESH)
    {
        // Input assembly is not used in the mesh pipeline, so topology may contain any value.
        // Validation layers may generate a warning if point_list topology is used, so use MAX_ENUM value.
        InputAssemblyCI.topology = VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;

        // Vertex input state and tessellation state are ignored in a mesh pipeline and should be null.
        PipelineCI.pVertexInputState = nullptr;
        PipelineCI.pTessellationState = nullptr;
    }
    else
    {
        PrimitiveTopology_To_VkPrimitiveTopologyAndPatchCPCount(GraphicsPipeline.PrimitiveTopology, InputAssemblyCI.topology, TessStateCI.patchControlPoints);
    }

    VkPipelineViewportStateCreateInfo ViewPortStateCI = {};

    ViewPortStateCI.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    ViewPortStateCI.pNext = nullptr;
    ViewPortStateCI.flags = 0; // reserved for future use
    ViewPortStateCI.viewportCount =
        GraphicsPipeline.NumViewports;                            // Even though we use dynamic viewports, the number of viewports used
    // by the pipeline is still specified by the viewportCount member (23.5)
    ViewPortStateCI.pViewports = nullptr;                       // We will be using dynamic viewport & scissor states
    ViewPortStateCI.scissorCount = ViewPortStateCI.viewportCount; // the number of scissors must match the number of viewports (23.5)
    // (why the hell it is in the struct then?)
    VkRect2D ScissorRect = {};
    if (GraphicsPipeline.RasterizerDesc.ScissorEnable)
    {
        ViewPortStateCI.pScissors = nullptr; // Ignored if the scissor state is dynamic
    }
    else
    {
        const auto& Props = PhysicalDevice.GetProperties();
        // There are limitiations on the viewport width and height (23.5), but
        // it is not clear if there are limitations on the scissor rect width and
        // height
        ScissorRect.extent.width = Props.limits.maxViewportDimensions[0];
        ScissorRect.extent.height = Props.limits.maxViewportDimensions[1];
        ViewPortStateCI.pScissors = &ScissorRect;
    }
    PipelineCI.pViewportState = &ViewPortStateCI;

    VkPipelineRasterizationStateCreateInfo RasterizerStateCI =
        RasterizerStateDesc_To_VkRasterizationStateCI(GraphicsPipeline.RasterizerDesc);
    PipelineCI.pRasterizationState = &RasterizerStateCI;

    // Multisample state (24)
    VkPipelineMultisampleStateCreateInfo MSStateCI = {};

    MSStateCI.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    MSStateCI.pNext = nullptr;
    MSStateCI.flags = 0; // reserved for future use
    // If subpass uses color and/or depth/stencil attachments, then the rasterizationSamples member of
    // pMultisampleState must be the same as the sample count for those subpass attachments
    MSStateCI.rasterizationSamples = static_cast<VkSampleCountFlagBits>(GraphicsPipeline.SmplDesc.Count);
    MSStateCI.sampleShadingEnable = VK_FALSE;
    MSStateCI.minSampleShading = 0;                                // a minimum fraction of sample shading if sampleShadingEnable is set to VK_TRUE.
    uint32_t SampleMask[] = { GraphicsPipeline.SampleMask, 0 }; // Vulkan spec allows up to 64 samples
    MSStateCI.pSampleMask = SampleMask;                       // an array of static coverage information that is ANDed with
    // the coverage information generated during rasterization (25.3)
    MSStateCI.alphaToCoverageEnable = VK_FALSE;                        // whether a temporary coverage value is generated based on
    // the alpha component of the fragment's first color output
    MSStateCI.alphaToOneEnable = VK_FALSE;                           // whether the alpha component of the fragment's first color output is replaced with one
    PipelineCI.pMultisampleState = &MSStateCI;

    VkPipelineDepthStencilStateCreateInfo DepthStencilStateCI =
        DepthStencilStateDesc_To_VkDepthStencilStateCI(GraphicsPipeline.DepthStencilDesc);
    PipelineCI.pDepthStencilState = &DepthStencilStateCI;

    const auto& RPDesc = pRenderPass->GetDesc();
    const auto  NumRTAttachments = RPDesc.pSubpasses[GraphicsPipeline.SubpassIndex].RenderTargetAttachmentCount;
    VERIFY_EXPR(GraphicsPipeline.pRenderPass != nullptr || GraphicsPipeline.NumRenderTargets == NumRTAttachments);
    std::vector<VkPipelineColorBlendAttachmentState> ColorBlendAttachmentStates(NumRTAttachments);

    VkPipelineColorBlendStateCreateInfo BlendStateCI = {};

    BlendStateCI.pAttachments = !ColorBlendAttachmentStates.empty() ? ColorBlendAttachmentStates.data() : nullptr;
    BlendStateCI.attachmentCount = NumRTAttachments; // must equal the colorAttachmentCount for the subpass
    // in which this pipeline is used.
    BlendStateDesc_To_VkBlendStateCI(GraphicsPipeline.BlendDesc, BlendStateCI, ColorBlendAttachmentStates);
    PipelineCI.pColorBlendState = &BlendStateCI;


    VkPipelineDynamicStateCreateInfo DynamicStateCI = {};

    DynamicStateCI.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    DynamicStateCI.pNext = nullptr;
    DynamicStateCI.flags = 0; // reserved for future use
    std::vector<VkDynamicState> DynamicStates =
    {
        VK_DYNAMIC_STATE_VIEWPORT, // pViewports state in VkPipelineViewportStateCreateInfo will be ignored and must be
        // set dynamically with vkCmdSetViewport before any draw commands. The number of viewports
        // used by a pipeline is still specified by the viewportCount member of
        // VkPipelineViewportStateCreateInfo.

VK_DYNAMIC_STATE_BLEND_CONSTANTS, // blendConstants state in VkPipelineColorBlendStateCreateInfo will be ignored
// and must be set dynamically with vkCmdSetBlendConstants

VK_DYNAMIC_STATE_STENCIL_REFERENCE // pecifies that the reference state in VkPipelineDepthStencilStateCreateInfo
                                   // for both front and back will be ignored and must be set dynamically
                                   // with vkCmdSetStencilReference
    };

    if (GraphicsPipeline.RasterizerDesc.ScissorEnable)
    {
        // pScissors state in VkPipelineViewportStateCreateInfo will be ignored and must be set
        // dynamically with vkCmdSetScissor before any draw commands. The number of scissor rectangles
        // used by a pipeline is still specified by the scissorCount member of
        // VkPipelineViewportStateCreateInfo.
        DynamicStates.push_back(VK_DYNAMIC_STATE_SCISSOR);
    }
    DynamicStateCI.dynamicStateCount = static_cast<uint32_t>(DynamicStates.size());
    DynamicStateCI.pDynamicStates = DynamicStates.data();
    PipelineCI.pDynamicState = &DynamicStateCI;


    PipelineCI.renderPass = pRenderPass.RawPtr<IRenderPassVk>()->GetVkRenderPass();
    PipelineCI.subpass = GraphicsPipeline.SubpassIndex;
    PipelineCI.basePipelineHandle = VK_NULL_HANDLE; // a pipeline to derive from
    PipelineCI.basePipelineIndex = -1;             // an index into the pCreateInfos parameter to use as a pipeline to derive from

    Pipeline = LogicalDevice.CreateGraphicsPipeline(PipelineCI, VK_NULL_HANDLE, PSODesc.Name);*/
}

VKPipelineState::VKPipelineState(VKRenderDevice& renderDevice, const GraphicsPipelineStateCreateInfo& createInfo)
: Base(renderDevice, createInfo)
{
	std::vector<VkPipelineShaderStageCreateInfo> vkShaderStages;
	std::vector<ShaderModuleWrapper> shaderModules;

	initInternalObjects(createInfo, vkShaderStages, shaderModules);

	createGraphicsPipeline(renderDevice, vkShaderStages, m_pipelineLayout, m_desc, getGraphicsPipelineDesc(), m_pipeline, *m_renderPass);
}

VKPipelineState::~VKPipelineState()
{
	// TODO
}

void initPipelineShaderStages(const VKLogicalDevice& logicalDevice,
	ShaderResourceLayoutVk::TShaderStages& shaderStages,
	std::vector<ShaderModuleWrapper>& shaderModules,
	std::vector<VkPipelineShaderStageCreateInfo>& stages)
{
    for (size_t s = 0; s < shaderStages.size(); ++s)
    {
        const auto& shaders = shaderStages[s].shaders;
        auto& SPIRVs = shaderStages[s].SPIRVs;
        const auto shaderType = shaderStages[s].type;

        POTATO_ASSERT(shaders.size() == SPIRVs.size());

        VkPipelineShaderStageCreateInfo stageCI = {};

        stageCI.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stageCI.pNext = nullptr;
        stageCI.flags = 0; //  reserved for future use
        stageCI.stage = shaderTypeToVkShaderStageFlagBit(shaderType);

        VkShaderModuleCreateInfo shaderModuleCI = {};

        shaderModuleCI.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderModuleCI.pNext = nullptr;
        shaderModuleCI.flags = 0;

        for (size_t i = 0; i < shaders.size(); ++i)
        {
            auto* shader = shaders[i];
            auto& SPIRV = SPIRVs[i];

            // We have to strip reflection instructions to fix the follownig validation error:
            //     SPIR-V module not valid: DecorateStringGOOGLE requires one of the following extensions: SPV_GOOGLE_decorate_string
            // Optimizer also performs validation and may catch problems with the byte code.
            //if (!StripReflection(LogicalDevice, SPIRV))
            //    LOG_ERROR("Failed to strip reflection information from shader '", pShader->GetDesc().Name, "'. This may indicate a problem with the byte code.");

            shaderModuleCI.codeSize = SPIRV.size() * sizeof(uint32_t);
            shaderModuleCI.pCode = SPIRV.data();

            shaderModules.push_back(logicalDevice.createShaderModule(shaderModuleCI, shader->getDesc().name.c_str()));

            stageCI.module = shaderModules.back();
            // TODO
            //stageCI.pName = shader->getEntryPoint();
            stageCI.pName = "main";
            stageCI.pSpecializationInfo = nullptr;

            stages.push_back(stageCI);
        }
    }

    POTATO_ASSERT(shaderModules.size() == stages.size());
}

template <typename PSOCreateInfoType>
VKPipelineState::TShaderStages VKPipelineState::initInternalObjects(
	const PSOCreateInfoType& createInfo,
	std::vector<VkPipelineShaderStageCreateInfo>& vkShaderStages,
	std::vector<ShaderModuleWrapper>& shaderModules)
{
	TShaderStages shaderStages;
	extractShaders<VKShader>(createInfo, shaderStages);

	const auto numShaderStages = getNumShaderStages();
	POTATO_ASSERT(numShaderStages > 0 && numShaderStages == shaderStages.size());

    reserveSpaceForPipelineDesc(createInfo);

	const auto& logicalDevice = GetDevice()->GetLogicalDevice();

	initializePipelineDesc(createInfo);

	initResourceLayouts(createInfo, shaderStages);

	// Create shader modules and initialize shader stages
	initPipelineShaderStages(logicalDevice, shaderStages, shaderModules, vkShaderStages);

    return shaderStages;
}

void VKPipelineState::initResourceLayouts(const PipelineStateCreateInfo& createInfo, TShaderStages& shaderStages)
{
    const auto& device = getDevice();
    const auto& logicalDevice = device.getVKLogicalDevice();

    for (size_t s = 0; s < shaderStages.size(); ++s)
    {
        auto& stageInfo = shaderStages[s];
        const auto shaderType = stageInfo.type;
        const auto shaderTypeInd = getShaderTypePipelineIndex(shaderType, m_desc.pipelineType);

        m_resourceLayoutIndex[shaderTypeInd] = static_cast<int8_t>(s);

        //auto& staticResLayout = m_shaderResourceLayouts[getNumShaderStages() + s];
        //staticResLayout.initializeStaticResourceLayout(stageInfo.shaders, getRawAllocator(), m_desc.resourceLayout, m_staticResCaches[s]);

        //m_staticVarsMgrs[s].initialize(staticResLayout, getRawAllocator(), nullptr, 0);
    }

    // Initialize shader resource layouts and assign bindings and descriptor sets in shader SPIRVs
    //ShaderResourceLayoutVk::initialize(device, shaderStages, m_shaderResourceLayouts/*, GetRawAllocator()*/,
    //    m_desc.resourceLayout, m_pipelineLayout,
    //    (createInfo.flags & PSO_CREATE_FLAG_IGNORE_MISSING_VARIABLES) == 0,
    //    (createInfo.flags & PSO_CREATE_FLAG_IGNORE_MISSING_IMMUTABLE_SAMPLERS) == 0);

    //m_pipelineLayout.finalize(logicalDevice);

    // TODO
}