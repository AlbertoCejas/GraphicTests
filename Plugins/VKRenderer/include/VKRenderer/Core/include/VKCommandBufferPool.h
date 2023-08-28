#pragma once

#include <atomic>
#include <cstdint>
#include <Core/include/VKLogicalDevice.h>
#include <Core/include/VKObjectWrapper.h>
#include <vulkan/vulkan_core.h>

namespace potato
{
	namespace vk
	{
		class VKLogicalDevice;

		class VKCommandBufferPool
		{
			public:

				VKCommandBufferPool(const VKLogicalDevice& logicalDevice, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags);
				~VKCommandBufferPool();

			private:

				const VKLogicalDevice* m_logicalDevice;
				CommandPoolWrapper m_cmdPool;
				std::atomic_int32_t m_buffCounter;
		};
	}
}