#include <Core/include/VKCommandQueue.h>
#include <Core/include/VKLogicalDevice.h>
#include <vulkan/vulkan_core.h>

using namespace potato;
using namespace potato::vk;


VKCommandQueue::VKCommandQueue(const VKLogicalDevice& logicalDevice, uint32_t queueFamilyIndex)
	: m_logicalDevice(logicalDevice)
	, m_VkQueue(logicalDevice.getQueue(queueFamilyIndex, 0))
	, m_queueFamilyIndex(queueFamilyIndex)
{
}

VKCommandQueue::~VKCommandQueue()
{

}
