#pragma once

#include <Core/include/VKLogicalDevice.h>


namespace potato
{
	namespace vk
	{
		// In 32-bit version, all Vulkan handles are typedefed as uint64_t, so we have to
		// use another way to distinguish objects.
		enum class VKHandleTypeId : uint32_t;

		template <typename VKObjectType, VKHandleTypeId>
		class VKObjectWrapper
		{
			public:

				using VkObjectType = VKObjectType;

				VKObjectWrapper()
					: m_logicalDevice(nullptr)
					, m_VKObject(VK_NULL_HANDLE)
				{}

				VKObjectWrapper(const VKLogicalDevice& logicalDevice, VKObjectType&& vkObject)
					: m_logicalDevice(&logicalDevice)
					, m_VKObject(vkObject)
				{
					vkObject = VK_NULL_HANDLE;
				}
				
				explicit VKObjectWrapper(VKObjectType vkObject)
					: m_VKObject(vkObject)
				{}

				VKObjectWrapper(VKObjectWrapper&& rhs)
					: m_logicalDevice(std::move(rhs.m_logicalDevice))
					, m_VKObject(rhs.m_VKObject)
				{
					rhs.m_VKObject = VK_NULL_HANDLE;
				}

				VKObjectWrapper& operator=(VKObjectWrapper&& rhs) noexcept
				{
					release();
					m_logicalDevice = std::move(rhs.m_logicalDevice);
					m_VKObject = rhs.m_VKObject;
					rhs.m_VKObject = VK_NULL_HANDLE;
					return *this;
				}

				operator VKObjectType() const
				{
					return m_VKObject;
				}

				const VKObjectType* operator&() const
				{
					return &m_VKObject;
				}

				void release()
				{
					// For externally managed objects, m_pLogicalDevice is null
					if (m_logicalDevice && m_VKObject != VK_NULL_HANDLE)
					{
						m_pLogicalDevice->releaseVulkanObject(std::move(*this));
					}
					m_VKObject = VK_NULL_HANDLE;
					m_logicalDevice = nullptr;
				}

			private:

				friend class VKLogicalDevice;

				const VKLogicalDevice* m_logicalDevice;
				VKObjectType m_VKObject;

		};
	}
}