#pragma once

#include <cstdint>
#include <PotatoEngine/Render/interface/IPipelineState.h>
#include <vector>

namespace potato
{
	namespace vk
	{
		class VKIRenderPass;

		class VKIPipelineState : public IPipelineState
		{
			public:

				/// Returns the pointer to the internal render pass object.
				virtual VKIRenderPass* getRenderPass() const = 0;

				/// Returns handle to a vulkan pipeline pass object.
				virtual VkPipeline getVkPipeline() const = 0;
		};
	}
}