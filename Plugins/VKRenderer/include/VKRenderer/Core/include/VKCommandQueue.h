#pragma once

#include <Core/interface/VKICommandQueue.h>


namespace potato
{
	namespace vk
	{
		class VKFence;
		class VKLogicalDevice;

		class VKCommandQueue final : public VKICommandQueue
		{
			public:

				using Base = VKICommandQueue;

				VKCommandQueue(const VKLogicalDevice& logicalDevice, uint32_t queueFamilyIndex);
				~VKCommandQueue() final;

				uint32_t getQueueFamilyIndex() const final { return m_queueFamilyIndex; }

				void setFence(VKFence& pFence) { m_fence = &pFence; }

			private:

				const VKLogicalDevice& m_logicalDevice;
				const VkQueue  m_VkQueue;
				const uint32_t m_queueFamilyIndex;
				VKFence* m_fence = nullptr;

		};
	}
}