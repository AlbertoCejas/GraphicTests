#pragma once

#include <Core/include/VKFencePool.h>
#include <Core/include/VKRenderDevice.h>
#include <Core/interface/VKIFence.h>
#include <cstdint>
#include <PotatoEngine/Render/include/BaseFence.h>
#include <queue>


namespace potato
{
	namespace vk
	{

		class VKFence final : public BaseFence<VKIFence, VKRenderDevice>
		{
			public:

				using Base = BaseFence<VKIFence, VKRenderDevice>;

				VKFence(VKRenderDevice& renderDevice, const FenceDesc& desc, bool isDeviceInternal = false);
				~VKFence() override;

				void addPendingFence(FenceWrapper&& vkFence, uint64_t FenceValue)
				{
					m_pendingFences.emplace_back(FenceValue, std::move(vkFence));
				}
				void wait(uint64_t value);

			private:

				VKFencePool m_fencePool;
				std::deque<std::pair<uint64_t, FenceWrapper>> m_pendingFences;
				volatile uint64_t m_lastCompletedFenceValue = 0;
		};
	}
}