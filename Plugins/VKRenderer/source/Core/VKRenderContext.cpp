#include <Core/include/VKRenderContext.h>
#include <Core/include/VKCommandQueue.h>
#include <Core/include/VKRenderDevice.h>
#include <Core/include/VKLogicalDevice.h>
#include <Core/Types.h>
#include <vulkan/vulkan_core.h>

using namespace potato;
using namespace potato::vk;


VKRenderContext::VKRenderContext(VKRenderDevice& renderDevice, uint32_t contextId, uint32_t commandQueueId)
	: VKRenderContextBaseType(renderDevice, contextId, commandQueueId)
	, m_commandBuffer(renderDevice.getVKLogicalDevice().getEnabledShaderStages())
	, m_commandPool(
		renderDevice.getVKLogicalDevice(), 
		renderDevice.getCommandQueue(commandQueueId).getQueueFamilyIndex(), 
		VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)

{
	//if (!m_bIsDeferred)
	//{
	//	m_QueryMgr.reset(new QueryManagerVk{ pDeviceVkImpl, EngineCI.QueryPoolSizes });
	//}

}

VKRenderContext::~VKRenderContext()
{
	// TODO
}
