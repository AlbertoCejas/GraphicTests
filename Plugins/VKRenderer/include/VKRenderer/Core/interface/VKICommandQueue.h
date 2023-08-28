#pragma once

#include <cstdint>

namespace potato
{
	namespace vk
	{
		class VKICommandQueue
		{
			public:

				virtual ~VKICommandQueue() {}

				virtual uint32_t getQueueFamilyIndex() const = 0;
		};
	}
}