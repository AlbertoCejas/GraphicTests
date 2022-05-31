#pragma once

#include <Core/interface/VKISwapChain.h>
#include <PotatoEngine/Render/include/BaseSwapChain.h>
#include <vulkan/vulkan_core.h>

class Window;

namespace potato
{
	namespace vk
	{
		class VKDeviceContext;
		class VKRenderDevice;

		class VKSwapChain final : public BaseSwapChain<VKISwapChain>
		{
			public:

				using Base = BaseSwapChain<VKISwapChain>;

				VKSwapChain(VKRenderDevice& renderDevice, Window& window);
				~VKSwapChain() override;

			private:

				void createSurface();
				void createVKSwapchain();
				void initBuffersAndViews();
				VkResult acquireNextImage(VKDeviceContext& deviceContext);

				VkInstance m_instance = VK_NULL_HANDLE;
				VkSurfaceKHR m_surface = VK_NULL_HANDLE;

				Window* m_window;
		};
	}
}