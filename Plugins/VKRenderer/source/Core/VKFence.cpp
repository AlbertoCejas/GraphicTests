#include <Core/include/VKFence.h>
#include <vulkan/vulkan_core.h>

using namespace potato;
using namespace potato::vk;


VKFence::VKFence(VKRenderDevice& renderDevice, const FenceDesc& desc, bool isDeviceInternal = false)
	: Base(renderDevice, desc, isDeviceInternal)
	, m_fencePool(renderDevice.getVKLogicalDevice())
{

}

VKFence::~VKFence()
{
    if (!m_pendingFences.empty())
    {
        //LOG_INFO_MESSAGE("FenceVkImpl::~FenceVkImpl(): waiting for ", m_pendingFences.size(), " pending Vulkan ",
        //    (m_pendingFences.size() > 1 ? "fences." : "fence."));

        // Vulkan spec states that all queue submission commands that refer to
        // a fence must have completed execution before the fence is destroyed.
        // (https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-vkDestroyFence-fence-01120)
        wait(UINT64_MAX);
    }
}

void VKFence::wait(uint64_t Value)
{
    const auto& logicalDevice = m_device.getVKLogicalDevice();
    while (!m_pendingFences.empty())
    {
        auto& val_fence = m_pendingFences.front();
        if (val_fence.first > Value)
            break;

        auto status = logicalDevice.getFenceStatus(val_fence.second);
        if (status == VK_NOT_READY)
        {
            VkFence FenceToWait = val_fence.second;

            status = logicalDevice.waitForFences(1, &FenceToWait, VK_TRUE, UINT64_MAX);
        }

        POTATO_ASSERT_MSG(status == VK_SUCCESS, "All pending fences must now be complete!");
        POTATO_UNUSED(status);
        if (val_fence.first > m_lastCompletedFenceValue)
            m_lastCompletedFenceValue = val_fence.first;
        m_fencePool.disposeFence(std::move(val_fence.second));

        m_pendingFences.pop_front();
    }
}
