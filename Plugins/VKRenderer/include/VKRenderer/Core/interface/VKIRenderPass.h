#pragma once

#include <PotatoEngine/Render/interface/IRenderPass.h>
#include <vulkan/vulkan_core.h>

namespace potato
{
	namespace vk
	{
		class VKIRenderPass : public IRenderPass
		{
			virtual VkRenderPass getVkRenderPass() const = 0;
		};
	}
}