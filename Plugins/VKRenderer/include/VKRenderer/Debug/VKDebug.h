#pragma once

#include <cstdint>
#include <vulkan/vulkan_core.h>


namespace potato
{
	namespace vk
	{
		// Load debug function pointers and set debug callback
		// if callBack is NULL, default message callback will be used
		void setupDebugging(VkInstance instance, VkDebugUtilsMessageSeverityFlagsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, void* pUserData = nullptr);
		// Clear debug callback
		void freeDebugging(VkInstance instance);


		// Sets the debug name of an object
		// All Objects in Vulkan are represented by their 64-bit handles which are passed into this function
		// along with the object type
		void setObjectName(VkDevice device, uint64_t object, VkObjectType objectType, const char* name);

		void setFenceName(VkDevice device, VkFence fence, const char* name);


		enum class VKHandleTypeId : uint32_t;
		template<typename VKObjectType, VKHandleTypeId>
		void setVulkanObjectName(VkDevice device, VKObjectType vkObject, const char* name);
	}
}