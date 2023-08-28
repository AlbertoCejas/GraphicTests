#include <Core/include/VKCommandBufferPool.h>
#include <PotatoEngine/Util/Assert.h>

using namespace potato;
using namespace potato::vk;


VKCommandBufferPool::VKCommandBufferPool(const VKLogicalDevice& logicalDevice, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags) 
    : m_logicalDevice{ &logicalDevice }
{
    VkCommandPoolCreateInfo cmdPoolCI = {};

    cmdPoolCI.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolCI.pNext = nullptr;
    cmdPoolCI.queueFamilyIndex = queueFamilyIndex;
    cmdPoolCI.flags = flags;

    m_cmdPool = m_logicalDevice->createCommandPool(cmdPoolCI);
    POTATO_ASSERT(m_cmdPool != VK_NULL_HANDLE, "Failed to create vulkan command pool");

    m_buffCounter = 0;
}

VKCommandBufferPool::~VKCommandBufferPool()
{
    m_cmdPool.release();
    POTATO_ASSERT(m_buffCounter == 0, " command buffer(s) have not been returned to the pool. If there are outstanding references to these buffers in release queues, FreeCommandBuffer() will crash when attempting to return a buffer to the pool.");
}