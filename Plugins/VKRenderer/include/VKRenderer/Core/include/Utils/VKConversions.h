#pragma once

#include <array>
#include <PotatoEngine/Render/include/GraphicsTypes.h>
#include <PotatoEngine/Render/interface/IRenderPass.h>
#include <PotatoEngine/Render/interface/IShader.h>
#include <vulkan/vulkan_core.h>


namespace potato
{
	namespace vk
	{
		VkFormat textureFormatToVkFormat(TextureFormat textureFormat);
		VkAttachmentLoadOp attachmentLoadOpToVkAttachmentLoadOp(AttachmentLoadOp LoadOp);
		VkAttachmentStoreOp attachmentStoreOpToVkAttachmentStoreOp(AttachmentStoreOp StoreOp);
		VkImageLayout resourceStateToVkImageLayout(ResourceState stateFlag, bool isInsideRenderPass = false);

		VkShaderStageFlagBits shaderTypeToVkShaderStageFlagBit(ShaderType shaderType);

		VkVertexInputRate layoutElemFrequencyToVkInputRate(InputElementFrequency frequency);
		VkFormat typeToVkFormat(ValueType valType, uint32_t numComponents, bool isNormalized);

		void inputLayoutDescToVkVertexInputStateCI(const InputLayoutDesc& layoutDesc,
			VkPipelineVertexInputStateCreateInfo& vertexInputStateCI,
			std::array<VkVertexInputBindingDescription, MAX_LAYOUT_ELEMENTS>& bindingDescriptions,
			std::array<VkVertexInputAttributeDescription, MAX_LAYOUT_ELEMENTS>& attributeDescription);

	}
}