#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>


namespace potato
{
	namespace vk
	{
		class VKPhysicalDevice
		{
			public:

				VKPhysicalDevice(VkPhysicalDevice physicalDevice);
				~VKPhysicalDevice();

				VkPhysicalDevice getVkPhysicalDevice() const { return m_physicalDevice; }

				uint32_t findQueueFamily(VkQueueFlags QueueFlags) const;
				bool checkPresentSupport(uint32_t queueFamilyIndex, VkSurfaceKHR VkSurface) const;

			private:

				VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

				//VkPhysicalDeviceProperties m_properties{};
				//VkPhysicalDeviceFeatures m_features{};
				//VkPhysicalDeviceMemoryProperties m_memoryProperties{};

				std::vector<VkQueueFamilyProperties> m_queueFamilyProperties;
		};
	}
}