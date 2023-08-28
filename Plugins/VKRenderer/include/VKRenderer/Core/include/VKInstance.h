#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>


namespace potato
{
	namespace vk
	{
		struct VKCreateInfo;

		class VKInstance
		{
			public:

				VKInstance(const VKCreateInfo& createInfo);
				~VKInstance();

				VkInstance getVkInstance() const { return m_instance; }

				VkPhysicalDevice pickPhysicalDevice(const std::vector<const char*>& deviceExtensions) const;

				bool isExtensionAvailable(const char* ExtensionName) const;

			private:

				bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers) const;
				std::vector<const char*> getRequiredExtensions(const VKCreateInfo& createInfo) const;
				void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) const;
				bool isDeviceSuitable(VkPhysicalDevice device) const;
				bool checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& deviceExtensions) const;

				VkInstance m_instance = VK_NULL_HANDLE;
				std::vector<VkExtensionProperties> m_extensions;
				bool m_debugUtilsEnabled = false;
		};
	}
}