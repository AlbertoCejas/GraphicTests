#pragma once

namespace potato
{
	template <class BaseInterface, typename ImplementationTraits>
	class BaseRenderContext : public BaseInterface
	{
		public:

			using DeviceType = typename ImplementationTraits::DeviceType;
			
			BaseRenderContext(DeviceType& renderDevice)
				: m_device(&renderDevice)
			{}

		protected:

			DeviceType* m_device = nullptr;
	};
}