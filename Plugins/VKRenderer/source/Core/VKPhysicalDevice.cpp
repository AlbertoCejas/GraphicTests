#include <Core/include/VKPhysicalDevice.h>
#include <PotatoEngine/Util/Assert.h>
#include <stdexcept>

using namespace potato;
using namespace potato::vk;


VKPhysicalDevice::VKPhysicalDevice(VkPhysicalDevice physicalDevice)
    : m_physicalDevice(physicalDevice)
{
    POTATO_ASSERT_MSG(m_physicalDevice != VK_NULL_HANDLE, "physical device is NULL");

    //vkGetPhysicalDeviceProperties(m_physicalDevice, &m_properties);
    //vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_features);
    //vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_memoryProperties);

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);
    m_queueFamilyProperties.resize(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, m_queueFamilyProperties.data());
}

VKPhysicalDevice::~VKPhysicalDevice()
{

}

uint32_t VKPhysicalDevice::findQueueFamily(VkQueueFlags queueFlags) const
{
    // All commands that are allowed on a queue that supports transfer operations are also allowed on
        // a queue that supports either graphics or compute operations. Thus, if the capabilities of a queue
        // family include VK_QUEUE_GRAPHICS_BIT or VK_QUEUE_COMPUTE_BIT, then reporting the VK_QUEUE_TRANSFER_BIT
        // capability separately for that queue family is optional (4.1).
    VkQueueFlags queueFlagsOpt = queueFlags;
    if (queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT))
    {
        queueFlags &= ~VK_QUEUE_TRANSFER_BIT;
        queueFlagsOpt = queueFlags | VK_QUEUE_TRANSFER_BIT;
    }

    static constexpr uint32_t InvalidFamilyIndex = std::numeric_limits<uint32_t>::max();
    uint32_t familyIndex = InvalidFamilyIndex;

    for (uint32_t i = 0; i < m_queueFamilyProperties.size(); ++i)
    {
        // First try to find a queue, for which the flags match exactly
        // (i.e. dedicated compute or transfer queue)
        const auto& props = m_queueFamilyProperties[i];
        if (props.queueFlags == queueFlags ||
            props.queueFlags == queueFlagsOpt)
        {
            familyIndex = i;
            break;
        }
    }

    if (familyIndex == InvalidFamilyIndex)
    {
        for (uint32_t i = 0; i < m_queueFamilyProperties.size(); ++i)
        {
            // Try to find a queue for which all requested flags are set
            const auto& props = m_queueFamilyProperties[i];
            // Check only QueueFlags as VK_QUEUE_TRANSFER_BIT is
            // optional for graphics and/or compute queues
            if ((props.queueFlags & queueFlags) == queueFlags)
            {
                familyIndex = i;
                break;
            }
        }
    }

    if (familyIndex == InvalidFamilyIndex)
    {
        throw std::runtime_error("Failed to find suitable queue family");
    }

    return familyIndex;
}

bool VKPhysicalDevice::checkPresentSupport(uint32_t queueFamilyIndex, VkSurfaceKHR VkSurface) const
{
    VkBool32 PresentSupport = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, queueFamilyIndex, VkSurface, &PresentSupport);
    return PresentSupport == VK_TRUE;
}