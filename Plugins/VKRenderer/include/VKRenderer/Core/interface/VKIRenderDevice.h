#pragma once

#include <PotatoEngine/Render/interface/IRenderDevice.h>
#include <VKRenderer/Core/Types.h>

namespace potato
{
	namespace vk
	{
		class VKIRenderDevice : public IRenderDevice
		{
			public:

				virtual ~VKIRenderDevice() {}

				virtual VkInstance GetVKInstance() = 0;
		};
	}
}