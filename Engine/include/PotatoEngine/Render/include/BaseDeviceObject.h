#pragma once

namespace potato
{

	template <class BaseInterface, class RenderDeviceImplType, typename ObjectDescType>
	class BaseDeviceObject : public BaseInterface
	{
		public:

			using Base = BaseInterface;

			BaseDeviceObject(RenderDeviceImplType& device, const ObjectDescType& objectDesc, bool isDeviceInternal = false)
				: Base(device, desc, isDeviceInternal)
				, m_desc(objectDesc)
				, m_isDeviceInternal(isDeviceInternal)
			{
			}

			virtual ~BaseDeviceObject()
			{
				if (m_isDeviceInternal == false)
				{
					delete& m_device;
				}
			}

			const ObjectDescType& getDesc() const final { return m_desc; }

			RenderDeviceImplType& getDevice() const { return m_device; }

		protected:

			/// Pointer to the device
			const RenderDeviceImplType& m_device;

			/// Object description
			ObjectDescType m_desc;

			const bool m_isDeviceInternal;

	};
}