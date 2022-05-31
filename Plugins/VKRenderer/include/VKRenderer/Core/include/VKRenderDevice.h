#pragma once

#include <Core/interface/VKIRenderDevice.h>
#include <PotatoEngine/Render/include/BaseRenderDevice.h>
#include <PotatoEngine/Render/include/BaseRenderDeviceLegacy.h>
#include <VKRenderer/Core/Types.h>
#include <vulkan/vulkan_core.h>

namespace potato
{
	namespace vk
	{
		class VKRenderDevice final : public BaseRenderDevice<BaseRenderDeviceLegacy<VKIRenderDevice>>
		{
			public:

				using Base = BaseRenderDevice<BaseRenderDeviceLegacy<VKIRenderDevice>>;

				VKRenderDevice(VkInstance vkInstance);
				~VKRenderDevice();

				VkInstance GetVKInstance() final { return m_instance; }

			private:

				VkInstance m_instance = VK_NULL_HANDLE;

		};
	}
}