#include <Core/include/VKSwapChain.h>

#include <algorithm>
#include <Core/include/VKPhysicalDevice.h>
#include <Core/include/VKRenderContext.h>
#include <Core/include/VKRenderDevice.h>
#include <Core/include/VKInstance.h>
#include <Core/interface/VKICommandQueue.h>
#include <Core/Types.h>

#include <PotatoEngine/Core/Window.h>

#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <vulkan/vulkan_core.h>

using namespace potato;
using namespace potato::vk;

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

// TODO: change to a non-glfw-oriented approach

VKSwapChain::VKSwapChain(VKRenderDevice& renderDevice, VKRenderContext& context, const SwapChainDesc& swapChainDesc, Window& window)
	: VKSwapChain::Base(renderDevice, context, swapChainDesc)
	, m_window(&window)
{
	m_VKInstance = &(renderDevice.getVKInstance());
	createSurface();
	createVulkanSwapChain();
	InitBuffersAndViews();
	auto res = AcquireNextImage(pDeviceContextVk);
	DEV_CHECK_ERR(res == VK_SUCCESS, "Failed to acquire next image for the newly created swap chain");
	(void)res;
}

VKSwapChain::~VKSwapChain()
{
}

void VKSwapChain::createSurface()
{
	POTATO_ASSERT_MSG(m_surface != VK_NULL_HANDLE, "Surface already exists, destroying the previous one before creating the new one");
	if (m_surface != VK_NULL_HANDLE)
	{
		vkDestroySurfaceKHR(m_VKInstance->getVkInstance(), m_surface, NULL);
		m_surface = VK_NULL_HANDLE;
	}

	const auto surfaceCreatedResult = glfwCreateWindowSurface(m_VKInstance->getVkInstance(), m_window->getNativeWindow(), nullptr, &m_surface);
	if (surfaceCreatedResult != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}

	const VKRenderDevice& renderDeviceImpl = static_cast<VKRenderDevice&>(m_renderDevice);
	const VKPhysicalDevice& physicalDevice = renderDeviceImpl.getVKPhysicalDevice();
	const VKICommandQueue& commandQueue = renderDeviceImpl.getCommandQueue(0);
	const uint32_t queueFamiliyIndex = commandQueue.getQueueFamilyIndex();
	if (physicalDevice.checkPresentSupport(queueFamiliyIndex, m_surface) == false)
	{
		throw std::runtime_error("Selected physical device does not support present capability.\n"
			"There could be few ways to mitigate this problem. One is to try to find another queue that supports present, but does not support graphics and compute capabilities."
			"Another way is to find another physical device that exposes queue family that supports present and graphics capability."
			"None of this is implemented in Potato Engine");
	}
}

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR& surface)
{
	SwapChainSupportDetails details;

	if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to get physical device surface capabilities!");
	}

	uint32_t formatCount;
	if (vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to get physical device surface format!");
	}
	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	if (vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to get physical device surface present modes count!");
	}
	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		if (vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to get physical device surface present modes!");
		}
	}

	return details;
}

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, const Window& window)
{
	if (capabilities.currentExtent.width != UINT32_MAX)
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width, height;
		glfwGetFramebufferSize(window.getNativeWindow(), &width, &height);

		VkExtent2D actualExtent =
		{
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

void VKSwapChain::createVulkanSwapChain()
{
	// TODO: handle headless support
	VKRenderDevice& renderDevice = static_cast<VKRenderDevice&>(m_renderDevice);
	VKPhysicalDevice& physicalDevice = renderDevice.getVKPhysicalDevice();
	VKLogicalDevice& logicalDevice = renderDevice.getVKLogicalDevice();
	VkPhysicalDevice physicalDeviceHandle = physicalDevice.getVkPhysicalDevice();
	VkDevice logicalDeviceHandle = logicalDevice.getVkDevice();

	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDeviceHandle, m_surface);
	const VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	const VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	const VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, *m_window);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = m_surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	// TODO? VS_SHARING_MODE_EXCLUSIVE is generally faster... 
	//QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);
	//uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	//
	//if (indices.graphicsFamily != indices.presentFamily)
	//{
	//	createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
	//	createInfo.queueFamilyIndexCount = 2;
	//	createInfo.pQueueFamilyIndices = queueFamilyIndices;
	//}
	//else
	//{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	//}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(logicalDeviceHandle, &createInfo, nullptr, &m_VKSwapChain) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create swap chain!");
	}

	// TODO: check errors
	vkGetSwapchainImagesKHR(logicalDeviceHandle, m_VKSwapChain, &imageCount, nullptr);
	m_swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(logicalDeviceHandle, m_VKSwapChain, &imageCount, m_swapChainImages.data());
	m_swapChainImageFormat = surfaceFormat.format;

	// views
	m_swapChainImageViews.resize(m_swapChainImages.size());
	for (uint32_t i = 0; i < m_swapChainImages.size(); i++)
	{
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = m_swapChainImages[i];
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = surfaceFormat.format;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		m_swapChainImageViews[i] = logicalDevice.createImageView(viewInfo);
	}
}