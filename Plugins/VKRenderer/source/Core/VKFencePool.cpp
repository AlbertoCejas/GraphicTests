#include <Core/include/VKFencePool.h>
#include <Core/include/VKFence.h>
#include <PotatoEngine/Util/Assert.h>
#include <vulkan/vulkan_core.h>

using namespace potato;
using namespace potato::vk;


VKFencePool::VKFencePool(VKLogicalDevice& logicalDevice)
	: m_logicalDevice(logicalDevice)
{}

VKFencePool::~VKFencePool()
{
	for (const auto& fence : m_fences)
	{
		POTATO_ASSERT_MSG(m_logicalDevice.getFenceStatus(fence) == VK_SUCCESS, "Destroying a fence that has not been signaled");
	}
	m_fences.clear();
}

FenceWrapper VKFencePool::getFence()
{
    FenceWrapper fence;
    if (!m_fences.empty())
    {
        fence = std::move(m_fences.back());
        m_logicalDevice.resetFence(fence);
        m_fences.pop_back();
    }
    else
    {
        VkFenceCreateInfo fenceCI = {};

        fenceCI.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCI.pNext = nullptr;
        fenceCI.flags = 0; // Available flag: VK_FENCE_CREATE_SIGNALED_BIT

        fence = m_logicalDevice.createFence(fenceCI);
    }
    return fence;
}

void VKFencePool::disposeFence(FenceWrapper&& fence)
{
    POTATO_ASSERT_MSG(m_logicalDevice.getFenceStatus(fence) == VK_SUCCESS, "Disposing a fence that has not been signaled");
    m_fences.emplace_back(std::move(fence));
}
