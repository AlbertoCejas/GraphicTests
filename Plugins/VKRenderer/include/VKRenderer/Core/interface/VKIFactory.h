#pragma once

#include <PotatoEngine/Render/interface/IGPUDriverFactory.h>

namespace potato
{
	class IRenderDevice;
	class IRenderContext;
	class ISwapChain;
	class Window;

	struct VKCreateInfo;
	struct SwapChainDesc;

	namespace vk
	{
		class VKIFactory : public IGPUDriverFactory
		{
			public:
				
				virtual void init(VKCreateInfo& createInfo) = 0;
				virtual void createSwapchain(IRenderDevice& renderDevice, IRenderContext& context, const SwapChainDesc& swapChainDesc, Window& window, ISwapChain* swapChain) = 0;
		};
	}
}