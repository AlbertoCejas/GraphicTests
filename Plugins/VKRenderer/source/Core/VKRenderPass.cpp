#include <Core/include/VKRenderPass.h>
#include <Core/include/Utils/VKConversions.h>
#include <vulkan/vulkan_core.h>

using namespace potato;
using namespace potato::vk;


VKRenderPass::VKRenderPass(VKRenderDevice& renderDevice, const RenderPassDesc& desc, bool isDeviceInternal = false)
	: Base(renderDevice, desc, isDeviceInternal)
{
    VkRenderPassCreateInfo renderPassCI = {};

    renderPassCI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCI.pNext = nullptr;
    renderPassCI.flags = 0;

    // attachments
    std::vector<VkAttachmentDescription> vkAttachments(desc.attachments.size());
    for (size_t i = 0; i < desc.attachments.size(); ++i)
    {
        const auto& attachment = m_desc.attachments[i];
        auto& vkAttachment = vkAttachments[i];
        vkAttachment.flags = 0;
        vkAttachment.format = textureFormatToVkFormat(attachment.format);
        vkAttachment.samples = static_cast<VkSampleCountFlagBits>(attachment.sampleCount);
        vkAttachment.loadOp = attachmentLoadOpToVkAttachmentLoadOp(attachment.loadOp);
        vkAttachment.storeOp = attachmentStoreOpToVkAttachmentStoreOp(attachment.storeOp);
        vkAttachment.stencilLoadOp = attachmentLoadOpToVkAttachmentLoadOp(attachment.stencilLoadOp);
        vkAttachment.stencilStoreOp = attachmentStoreOpToVkAttachmentStoreOp(attachment.stencilStoreOp);
        vkAttachment.initialLayout = resourceStateToVkImageLayout(attachment.initialState, /*isInsideRenderPass = */ false);
        vkAttachment.finalLayout = resourceStateToVkImageLayout(attachment.finalState, /*isInsideRenderPass = */ true);
    }
    renderPassCI.attachmentCount = desc.attachments.size();
    renderPassCI.pAttachments = vkAttachments.data();

    // subpasses
    uint32_t totalAttachmentReferencesCount = 0;
    countSubpassAttachmentReferences(desc, totalAttachmentReferencesCount/*, totalPreserveAttachmentsCount*/);
    std::vector<VkAttachmentReference> vkAttachmentReferences(totalAttachmentReferencesCount);
    uint32_t currAttachmentReferenceInd = 0;
    //uint32_t currPreserveAttachmentInd = 0;

    std::vector<VkSubpassDescription> vkSubpasses(desc.subpasses.size());
    for (size_t i = 0; i < desc.subpasses.size(); ++i)
    {
        const auto& subpassDesc = m_desc.subpasses[i];
        auto& vkSubpass = vkSubpasses[i];
        vkSubpass.flags = 0;
        vkSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        //vkSubpass.inputAttachmentCount = subpassDesc.inputAttachmentCount;

        auto ConvertAttachmentReferences = [&](uint32_t numAttachments, const AttachmentReference* pSrcAttachments) //
            {
                auto* pCurrVkAttachmentReference = &vkAttachmentReferences[currAttachmentReferenceInd];
                for (uint32_t attachment = 0; attachment < numAttachments; ++attachment, ++currAttachmentReferenceInd)
                {
                    const auto& AttachmnetRef = pSrcAttachments[attachment];

                    vkAttachmentReferences[currAttachmentReferenceInd].attachment = AttachmnetRef.attachmentIndex;
                    vkAttachmentReferences[currAttachmentReferenceInd].layout = resourceStateToVkImageLayout(AttachmnetRef.state, /*IsInsideRenderPass = */ true);
                }

                return pCurrVkAttachmentReference;
            };

        /*
        if (SubpassDesc.InputAttachmentCount != 0)
        {
            vkSubpass.pInputAttachments = ConvertAttachmentReferences(SubpassDesc.InputAttachmentCount, SubpassDesc.pInputAttachments);
        }
        */

        vkSubpass.colorAttachmentCount = subpassDesc.renderTargetAttachments.size();
        if (subpassDesc.renderTargetAttachments.empty() == false)
        {
            vkSubpass.pColorAttachments = ConvertAttachmentReferences(subpassDesc.renderTargetAttachments.size(), subpassDesc.renderTargetAttachments.data());
            if (subpassDesc.resolveAttachments.empty() == false)
            {
                vkSubpass.pResolveAttachments = ConvertAttachmentReferences(subpassDesc.resolveAttachments.size(), subpassDesc.resolveAttachments.data());
            }
        }

        if (subpassDesc.depthStencilAttachment != nullptr)
        {
            vkSubpass.pDepthStencilAttachment = ConvertAttachmentReferences(1, subpassDesc.depthStencilAttachment);
        }

        /*
        vkSubpass.preserveAttachmentCount = SubpassDesc.PreserveAttachmentCount;
        if (SubpassDesc.PreserveAttachmentCount != 0)
        {
            vkSubpass.pPreserveAttachments = &vkPreserveAttachments[CurrPreserveAttachmentInd];
            for (Uint32 prsv_attachment = 0; prsv_attachment < SubpassDesc.PreserveAttachmentCount; ++prsv_attachment, ++CurrPreserveAttachmentInd)
            {
                vkPreserveAttachments[CurrPreserveAttachmentInd] = SubpassDesc.pPreserveAttachments[prsv_attachment];
            }
        }
        */
    }
    POTATO_ASSERT(currAttachmentReferenceInd == vkAttachmentReferences.size());
    //VERIFY_EXPR(CurrPreserveAttachmentInd == vkPreserveAttachments.size());
    renderPassCI.subpassCount = desc.subpasses.size();
    renderPassCI.pSubpasses = vkSubpasses.data();

    // Dependencies
    std::vector<VkSubpassDependency> vkDependencies(desc.dependencies.size());
    for (size_t i = 0; i < desc.dependencies.size(); ++i)
    {
        const auto& dependencyDesc = m_desc.dependencies[i];
        auto& vkDependency = vkDependencies[i];
        vkDependency.srcSubpass = dependencyDesc.srcSubpass;
        vkDependency.dstSubpass = dependencyDesc.dstSubpass;
        vkDependency.srcStageMask = dependencyDesc.srcStageMask;
        vkDependency.dstStageMask = dependencyDesc.dstStageMask;
        vkDependency.srcAccessMask = dependencyDesc.srcAccessMask;
        vkDependency.dstAccessMask = dependencyDesc.dstAccessMask;

        // VK_DEPENDENCY_BY_REGION_BIT specifies that dependencies will be framebuffer-local.
        // Framebuffer-local dependencies are more optimal for most architectures; particularly
        // tile-based architectures - which can keep framebuffer-regions entirely in on-chip registers
        // and thus avoid external bandwidth across such a dependency. Including a framebuffer-global
        // dependency in your rendering will usually force all implementations to flush data to memory,
        // or to a higher level cache, breaking any potential locality optimizations.
        vkDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
    }
    renderPassCI.dependencyCount = desc.dependencies.size();
    renderPassCI.pDependencies = vkDependencies.data();

    const auto& logicalDevice = renderDevice.getVKLogicalDevice();

    m_VkRenderPass = logicalDevice.createRenderPass(renderPassCI, desc.name.c_str());
    POTATO_ASSERT_MSG(m_VkRenderPass != VK_NULL_HANDLE, "Failed to create Vulkan render pass");
}

VKRenderPass::~VKRenderPass()
{
    // TODO: delete m_VkRenderPass
}

