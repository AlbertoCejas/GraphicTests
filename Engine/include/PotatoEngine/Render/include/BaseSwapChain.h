#pragma once

#include <PotatoEngine/Render/include/GraphicsTypes.h>

namespace potato
{
	class IRenderContext;
	class IRenderDevice;

	template <class BaseInterface>
	class BaseSwapChain : public BaseInterface
	{
		public:

			BaseSwapChain(IRenderDevice& device,
				IRenderContext& context,
				const SwapChainDesc& swapChainDesc)
				: m_renderDevice(device)
				, m_renderContext(context)
				, m_swapChainDesc(swapChainDesc) {}

			virtual ~BaseSwapChain() override {}

			const SwapChainDesc& getDesc() const final { return m_swapChainDesc; }

		protected:

			IRenderDevice& m_renderDevice;
			IRenderContext& m_renderContext;
			SwapChainDesc m_swapChainDesc;

	};
}