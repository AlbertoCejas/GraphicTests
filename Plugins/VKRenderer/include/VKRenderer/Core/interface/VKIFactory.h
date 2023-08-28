#pragma once

#include <PotatoEngine/Render/interface/IGPUDriverFactory.h>

namespace potato
{
	class Window;

	struct SwapChainDesc;

	namespace vk
	{
		struct VKCreateInfo;

		class VKRenderDevice;
		class VKRenderContext;
		class VKSwapChain;

		class VKIFactory : public IGPUDriverFactory
		{
			public:
				
				virtual VKRenderDevice* createRenderDevice(VKCreateInfo& createInfo) = 0;
				virtual VKRenderContext* createRenderContexts(VKRenderDevice& renderDevice) = 0;
				virtual VKSwapChain* createSwapchain(VKRenderDevice& renderDevice, VKRenderContext& context, const SwapChainDesc& swapChainDesc, Window& window) = 0;
		};
	}
}