#pragma once

#include <Core/include/VKCommandBuffer.h>
#include <Core/include/VKCommandBufferPool.h>
#include <Core/interface/VKIRenderContext.h>
#include <PotatoEngine/Render/include/BaseRenderContext.h>
#include <PotatoEngine/Render/include/BaseRenderContextModern.h>
#include <Core/Types.h>

namespace potato
{
	namespace vk
	{
		class VKRenderDevice;

		struct VKDeviceContextTraits
		{
			//using BufferType = BufferVkImpl;
			//using TextureType = TextureVkImpl;
			//using PipelineStateType = PipelineStateVkImpl;
			using DeviceType = VKIRenderDevice;
			//using ICommandQueueType = ICommandQueueVk;
			//using QueryType = QueryVkImpl;
			//using FramebufferType = FramebufferVkImpl;
			//using RenderPassType = RenderPassVkImpl;
			//using BottomLevelASType = BottomLevelASVkImpl;
			//using TopLevelASType = TopLevelASVkImpl;
		};

		class VKRenderContext final : public BaseRenderContextModern<VKIRenderContext, VKDeviceContextTraits>
		{
			public:

				using VKRenderContextBaseType = BaseRenderContextModern<VKIRenderContext, VKDeviceContextTraits>;

				VKRenderContext(VKRenderDevice& renderDevice, uint32_t contextId, uint32_t commandQueueId);
				~VKRenderContext();

			private:

				VKCommandBuffer m_commandBuffer;
				VKCommandBufferPool m_commandPool;

		};
	}
}