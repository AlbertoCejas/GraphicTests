#pragma once

#include <Core/interface/VKIFactory.h>
#include <PotatoEngine/Render/include/BaseGPUDriverFactory.h>
#include <VKRenderer/Core/Types.h>
#include <vulkan/vulkan_core.h>

namespace potato
{
	class Window;

	namespace vk
	{
		class VKInstance;
		class VKLogicalDevice;
		class VKPhysicalDevice;
		class VKIRenderDevice;
		class VKIRenderContext;
		class VKISwapChain;

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
				};
		        */

				~VKFactory();

				VKRenderDevice* createRenderDevice(VKCreateInfo& createInfo) final;
				VKRenderContext* createRenderContexts(VKRenderDevice& renderDevice) final;
				VKSwapChain* createSwapchain(VKRenderDevice& renderDevice, VKRenderContext& context, const SwapChainDesc& swapChainDesc, Window& window) final;

			private:

		        void fillRequiredDeviceExtensions(std::vector<const char*>& deviceExtensions) const;

		        VkDebugUtilsMessengerEXT m_debugMessenger;

		};
	}
}