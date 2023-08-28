#pragma once

#include <vulkan/vulkan_core.h>

namespace potato
{
	namespace vk
	{

		class VKCommandBuffer
		{
			public:

				VKCommandBuffer(VkPipelineStageFlags enabledShaderStages) 
					: m_enabledShaderStages{ enabledShaderStages }
				{}

				void setVkCmdBuffer(VkCommandBuffer vkCmdBuffer)
				{
					m_vkCmdBuffer = vkCmdBuffer;
				}
				VkCommandBuffer getVkCmdBuffer() const { return m_vkCmdBuffer; }

				VkPipelineStageFlags getEnabledShaderStages() const { return m_enabledShaderStages; }

			private:

				VkCommandBuffer m_vkCmdBuffer = VK_NULL_HANDLE;
				const VkPipelineStageFlags m_enabledShaderStages;

		};
	}
}