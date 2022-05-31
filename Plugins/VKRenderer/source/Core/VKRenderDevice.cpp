#include <Core/include/VKRenderDevice.h>
#include <Core/Types.h>
#include <vulkan/vulkan_core.h>

using namespace potato;
using namespace potato::vk;


VKRenderDevice::VKRenderDevice(VkInstance vkInstance)
	: VKRenderDevice::Base()
	, m_instance(vkInstance)
{
}

VKRenderDevice::~VKRenderDevice()
{
}
