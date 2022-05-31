#pragma once

#include <PotatoEngine/Render/interface/ISwapChain.h>

namespace potato
{
	namespace vk
	{
		class VKISwapChain : public ISwapChain
		{
			public:

				virtual ~VKISwapChain() {}
		};
	}
}