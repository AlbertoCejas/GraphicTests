#pragma once

#include <Core/interface/VKIFactory.h>
#include <PotatoEngine/Render/include/BaseGPUDriverFactory.h>
#include <VKRenderer/Core/Types.h>
#include <vulkan/vulkan_core.h>

namespace potato
{

	namespace vk
	{
		class VKIRenderDevice;
		class VKIRenderContext;

		class VKFactory final : public BaseGPUDriverFactory<VKIFactory>
		{
			public:

		        /*
		        TODO:

		        // to pass within createInfo
		        const std::vector<const char*> validationLayers =
		        {
		            "VK_LAYER_KHRONOS_validation"
		        };

		        const std::vector<const char*> deviceExtensions =
		        {
		            VK_KHR_SWAPCHAIN_EXTENSION_NAME
		        };
		        
		        appInfo.applicationVersion -> 0, 1, 0, 0;
		        appInfo.engineVersion      -> 0, 1, 0, 0;

		        // IF USING GLFW, ADD IT TO EXTENSIONS:
		        uint32_t glfwExtensionCount = 0;
		        const char** glfwExtensions;
		        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		        */

				~VKFactory();

		        void init(VKCreateInfo& createInfo) final;
				void createSwapchain(IRenderDevice& renderDevice, IRenderContext& context, const SwapChainDesc& swapChainDesc, Window& window, ISwapChain* swapChain) final;

			private:

				void createInstance(const VKCreateInfo& createInfo);
		        std::vector<const char*> getRequiredExtensions(const VKCreateInfo& createInfo) const;
		        bool checkValidationLayerSupport(const std::vector<const char*>& validationLayerNames) const;

		        void setupDebugMessenger(const VKCreateInfo& createInfo);
		        VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) const;
		        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) const;

		        void fillRequiredDeviceExtensions(std::vector<const char*>& deviceExtensions) const;

		        void pickPhysicalDevice(const std::vector<const char*>& deviceExtensions);
		        bool isDeviceSuitable(VkPhysicalDevice device, const std::vector<const char*>& deviceExtensions) const;
		        bool checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& deviceExtensions) const;

		        void createLogicalDevice(const VKCreateInfo& createInfo);

		        void createRenderDevice(const VKCreateInfo& createInfo);

				void createRenderContext(const VKCreateInfo& createInfo);

		        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;



		        VkInstance m_instance = VK_NULL_HANDLE;
		        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
		        VkDevice m_logicalDevice = VK_NULL_HANDLE;
		        VkQueue m_graphicsQueue = VK_NULL_HANDLE;
		        VkQueue m_computeQueue = VK_NULL_HANDLE;

		        VkDebugUtilsMessengerEXT m_debugMessenger;

				VKIRenderDevice* m_renderDevice = nullptr;
				VKIRenderContext* m_renderContext = nullptr;

		};
	}
}