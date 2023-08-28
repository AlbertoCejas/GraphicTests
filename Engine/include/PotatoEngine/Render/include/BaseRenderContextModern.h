#pragma once

#include <cstdint>
#include <PotatoEngine/Render/include/BaseRenderContext.h>

namespace potato
{
	template <class BaseInterface, typename ImplementationTraits>
	class BaseRenderContextModern : public BaseRenderContext<BaseInterface, ImplementationTraits>
	{
		public:

			using Base = BaseRenderContext<BaseInterface, ImplementationTraits>;
			using DeviceType = typename ImplementationTraits::DeviceType;

			BaseRenderContextModern(DeviceType& renderDevice, uint32_t contextId, uint32_t commandQueueId)
				: Base(renderDevice)
				, m_contextId(contextId)
				, m_commandQueueId(commandQueueId)
			{}

		private:

			const uint32_t m_contextId;
			const uint32_t m_commandQueueId;
	};
}