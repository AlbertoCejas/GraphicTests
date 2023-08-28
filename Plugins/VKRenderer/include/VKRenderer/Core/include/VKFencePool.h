#pragma once

#include <Core/include/VKObjectWrapper.h>
#include <vector>


namespace potato
{
	namespace vk
	{
		class VKFencePool
		{
			public:

				VKFencePool(VKLogicalDevice& vkLogicalDevice);
				~VKFencePool();

				FenceWrapper getFence();
				void disposeFence(FenceWrapper&& fence);

			private:

				const VKLogicalDevice& m_logicalDevice;
				std::vector<FenceWrapper> m_fences;

		};
	}
}