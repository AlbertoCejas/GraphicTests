#pragma once

#include <Core/interface/VKISwapChain.h>
#include <PotatoEngine/Render/include/BaseSwapChain.h>
#include <vulkan/vulkan_core.h>
#include <vector>


namespace potato
{
	class Window;

	namespace vk
	{
		class VKInstance;
		class VKRenderDevice;
		class VKRenderContext;

		class VKSwapChain final : public BaseSwapChain<VKISwapChain>
		{
			public:

				using Base = BaseSwapChain<VKISwapChain>;

				VKSwapChain(VKRenderDevice& renderDevice, VKRenderContext& context, const SwapChainDesc& swapChainDesc, Window& window);
				~VKSwapChain() override;

			private:

				void createSurface();
				void createVulkanSwapChain();

				VKInstance* m_VKInstance = nullptr;
				VkSurfaceKHR m_surface = VK_NULL_HANDLE;
				VkSwapchainKHR m_VKSwapChain = VK_NULL_HANDLE;
				std::vector<VkImage> m_swapChainImages;
				VkFormat m_swapChainImageFormat;
				std::vector<VkImageView> m_swapChainImageViews;
				Window* m_window = nullptr;
		};
	}
}