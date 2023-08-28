#include <Core/include/VKRenderDevice.h>

#include <Core/include/VKInstance.h>
#include <Core/include/VKLogicalDevice.h>
#include <Core/include/VKPhysicalDevice.h>
#include <Core/include/VKRenderPass.h>
#include <Core/include/VKShader.h>
#include <Core/Types.h>
#include <PotatoEngine/Util/Assert.h>
#include <vulkan/vulkan_core.h>

using namespace potato;
using namespace potato::vk;


VKRenderDevice::VKRenderDevice(VKInstance* instance, VKPhysicalDevice* physicalDevice, VKLogicalDevice* logicalDevice, size_t commandQueueCount, VKICommandQueue** commandQueues)
	: VKRenderDevice::Base(commandQueueCount, commandQueues)
	, m_instance(instance)
	, m_physicalDevice(physicalDevice)
	, m_logicalDevice(logicalDevice)
{
	POTATO_ASSERT_MSG(instance != nullptr, "VKInstance is nullptr");
	POTATO_ASSERT_MSG(physicalDevice != nullptr, "VKPhysicalDevice is nullptr");
	POTATO_ASSERT_MSG(logicalDevice != nullptr, "VKLogicalDevice is nullptr");
}

VKRenderDevice::~VKRenderDevice()
{
	delete m_logicalDevice;
	delete m_physicalDevice;
	delete m_instance;
}

IRenderPass* VKRenderDevice::createRenderPass(const RenderPassDesc& desc)
{
	return createRenderPass(desc, false);
}

IRenderPass* VKRenderDevice::createRenderPass(const RenderPassDesc& desc, bool isDeviceInternal)
{
	return new VKRenderPass(*this, desc, isDeviceInternal);
}

IShader* VKRenderDevice::createShader(const ShaderCreateInfo& shaderCI)
{
	return new VKShader(*this, shaderCI);
}