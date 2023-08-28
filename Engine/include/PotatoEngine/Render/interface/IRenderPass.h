#pragma once

#include <string>
#include <PotatoEngine/Render/include/GraphicsTypes.h>
#include <vector>

namespace potato
{
    /// Render pass attachment load operation
    /// Vulkan counterpart: [VkAttachmentLoadOp](https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VkAttachmentLoadOp).
    /// D3D12 counterpart: [D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE](https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ne-d3d12-d3d12_render_pass_beginning_access_type).
    enum AttachmentLoadOp : uint8_t
    {
        /// The previous contents of the texture within the render area will be preserved.
        /// Vulkan counterpart: VK_ATTACHMENT_LOAD_OP_LOAD.
        /// D3D12 counterpart: D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_PRESERVE.
        ATTACHMENT_LOAD_OP_LOAD = 0,

        /// The contents within the render area will be cleared to a uniform value, which is
        /// specified when a render pass instance is begun.
        /// Vulkan counterpart: VK_ATTACHMENT_LOAD_OP_CLEAR.
        /// D3D12 counterpart: D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR.
        ATTACHMENT_LOAD_OP_CLEAR,

        /// The previous contents within the area need not be preserved; the contents of
        /// the attachment will be undefined inside the render area.
        /// Vulkan counterpart: VK_ATTACHMENT_LOAD_OP_DONT_CARE.
        /// D3D12 counterpart: D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_DISCARD.
        ATTACHMENT_LOAD_OP_DISCARD
    };

    /// Render pass attachment store operation
    /// Vulkan counterpart: [VkAttachmentStoreOp](https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VkAttachmentStoreOp).
    /// D3D12 counterpart: [D3D12_RENDER_PASS_ENDING_ACCESS_TYPE](https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ne-d3d12-d3d12_render_pass_ending_access_type).
    enum AttachmentStoreOp : uint8_t
    {
        /// The contents generated during the render pass and within the render area are written to memory.
        /// Vulkan counterpart: VK_ATTACHMENT_STORE_OP_STORE.
        /// D3D12 counterpart: D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE.
        ATTACHMENT_STORE_OP_STORE = 0,

        /// The contents within the render area are not needed after rendering, and may be discarded;
        /// the contents of the attachment will be undefined inside the render area.
        /// Vulkan counterpart: VK_ATTACHMENT_STORE_OP_DONT_CARE.
        /// D3D12 counterpart: D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_DISCARD.
        ATTACHMENT_STORE_OP_DISCARD
    };

    /// Render pass attachment description.
    struct RenderPassAttachmentDesc
    {
        /// The format of the texture view that will be used for the attachment.
        TextureFormat format = TextureFormat::TEX_FORMAT_UNKNOWN;
        
        /// The number of samples in the texture.
        uint8_t sampleCount = 1;

        /// Load operation that specifies how the contents of color and depth components of
        /// the attachment are treated at the beginning of the subpass where it is first used.
        AttachmentLoadOp loadOp = AttachmentLoadOp::ATTACHMENT_LOAD_OP_LOAD;

        /// Store operation how the contents of color and depth components of the attachment
        /// are treated at the end of the subpass where it is last used.
        AttachmentStoreOp storeOp = AttachmentStoreOp::ATTACHMENT_STORE_OP_STORE;

        /// Load operation that specifies how the contents of the stencil component of the
        /// attachment is treated at the beginning of the subpass where it is first used.
        /// This value is ignored when the format does not have stencil component.
        AttachmentLoadOp stencilLoadOp = AttachmentLoadOp::ATTACHMENT_LOAD_OP_LOAD;

        /// Store operation how the contents of the stencil component of the attachment
        /// is treated at the end of the subpass where it is last used.
        /// This value is ignored when the format does not have stencil component.
        AttachmentStoreOp stencilStoreOp = AttachmentStoreOp::ATTACHMENT_STORE_OP_STORE;

        /// The state the attachment texture subresource will be in when a render pass instance begins.
        ResourceState initialState = ResourceState::RESOURCE_STATE_UNKNOWN;

        /// The state the attachment texture subresource will be transitioned to when a render pass instance ends.
        ResourceState finalState = ResourceState::RESOURCE_STATE_UNKNOWN;
    };

    /// Attachment reference description.
    struct AttachmentReference
    {
        /// Either an integer value identifying an attachment at the corresponding index in RenderPassDesc::pAttachments,
        /// or ATTACHMENT_UNUSED to signify that this attachment is not used.
        uint32_t attachmentIndex = 0;

        /// The state of the attachment during the subpass.
        ResourceState state = ResourceState::RESOURCE_STATE_UNKNOWN;
    };

    /// Render pass subpass decription.
    struct SubpassDesc
    {
        /// The number of input attachments the subpass uses.
        //uint32_t inputAttachmentCount = 0;

        /// Pointer to the array of input attachments, see PotatoEngine::AttachmentReference.
        //const AttachmentReference* inputAttachments = nullptr;

        /// Vector of color render target attachments, see PotatoEngine::AttachmentReference.

        /// Each element of the pRenderTargetAttachments array corresponds to an output in the pixel shader,
        /// i.e. if the shader declares an output variable decorated with a render target index X, then it uses
        /// the attachment provided in pRenderTargetAttachments[X]. If the attachment index is ATTACHMENT_UNUSED,
        /// writes to this render target are ignored.
        std::vector<const AttachmentReference> renderTargetAttachments;

        /// Vector of resolve attachments, see PotatoEngine::AttachmentReference.

        /// If pResolveAttachments is not NULL, each of its elements corresponds to a render target attachment
        /// (the element in pRenderTargetAttachments at the same index), and a multisample resolve operation is
        /// defined for each attachment. At the end of each subpass, multisample resolve operations read the subpass's
        /// color attachments, and resolve the samples for each pixel within the render area to the same pixel location
        /// in the corresponding resolve attachments, unless the resolve attachment index is ATTACHMENT_UNUSED.
        std::vector<const AttachmentReference> resolveAttachments;

        /// Pointer to the depth-stencil attachment, see PotatoEngine::AttachmentReference.
        const AttachmentReference* depthStencilAttachment = nullptr;

        /// The number of preserve attachments.
        //uint32_t preserveAttachmentCount = 0;

        /// Pointer to the array of preserve attachments, see PotatoEngine::AttachmentReference.
        //const uint32_t* preserveAttachments = nullptr;
    };

    #define SUBPASS_EXTERNAL (~0U)

    /// Subpass dependency description
    struct SubpassDependencyDesc
    {
        /// The subpass index of the first subpass in the dependency, or SUBPASS_EXTERNAL.
        uint32_t srcSubpass = 0;

        /// The subpass index of the second subpass in the dependency, or SUBPASS_EXTERNAL.
        uint32_t dstSubpass = 0;

        /// A bitmask of PIPELINE_STAGE_FLAGS specifying the source stage mask.
        PipelineStageFlags  srcStageMask = PipelineStageFlags::PIPELINE_STAGE_FLAG_UNDEFINED;

        /// A bitmask of PIPELINE_STAGE_FLAGS specifying the destination stage mask.
        PipelineStageFlags  dstStageMask = PipelineStageFlags::PIPELINE_STAGE_FLAG_UNDEFINED;

        /// A bitmask of ACCESS_FLAGS specifying a source access mask.
        AccessFlags srcAccessMask = AccessFlags::ACCESS_FLAG_NONE;

        /// A bitmask of ACCESS_FLAGS specifying a destination access mask.
        AccessFlags dstAccessMask = AccessFlags::ACCESS_FLAG_NONE;
    };

	struct RenderPassDesc
	{
		std::string name = nullptr;

        /// Vector of subpass attachments, see PotatoEngine::RenderPassAttachmentDesc.
        std::vector<const RenderPassAttachmentDesc> attachments;

        /// Vector of subpass descriptions, see PotatoEngine::SubpassDesc.
        std::vector<const SubpassDesc> subpasses;

        /// Vector of subpass dependencies, see PotatoEngine::SubpassDependencyDesc.
        std::vector<const SubpassDependencyDesc> dependencies;
	};

	class IRenderPass
	{
		public:

			virtual const RenderPassDesc& getDesc() const = 0;
	};
}