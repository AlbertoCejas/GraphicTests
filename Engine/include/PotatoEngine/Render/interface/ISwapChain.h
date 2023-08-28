#pragma once

namespace potato
{
	struct SwapChainDesc;

	class ISwapChain
	{
		public:

			virtual ~ISwapChain() {}

			virtual const SwapChainDesc& getDesc() const = 0;
	};
}