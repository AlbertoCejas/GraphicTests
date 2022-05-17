#pragma once

#include <cstdint>
#include <optional>
#include <vector>

namespace potato
{
	namespace vk
	{
		struct VKCreateInfo
		{
			const char* appName;
			uint32_t appVersion[4];
			const char* engineName;
			uint32_t engineVersion[4];

			bool enableValidation;
			std::vector<const char*> validationLayerNames;
			std::vector<const char*> extensionNames;
		};

		struct QueueFamilyIndices
		{
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;

			bool isComplete() const
			{
				return graphicsFamily.has_value() && presentFamily.has_value();
			}
		};

		struct SwapChainSupportDetails
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};
	}
}