#pragma once

#include <PotatoEngine/Render/interface/IRenderDevice.h>
#include <VKRenderer/Core/Types.h>
#include <vulkan/vulkan_core.h>

namespace potato
{
	namespace vk
	{
		class VKInstance;
		class VKPhysicalDevice;
		class VKLogicalDevice;

		class VKIRenderDevice : public IRenderDevice
		{
			public:

				virtual ~VKIRenderDevice() {};

				virtual VKInstance& getVKInstance() const = 0;
				virtual VKPhysicalDevice& getVKPhysicalDevice() const = 0;
				virtual VKLogicalDevice& getVKLogicalDevice() const = 0;
		};
	}
}