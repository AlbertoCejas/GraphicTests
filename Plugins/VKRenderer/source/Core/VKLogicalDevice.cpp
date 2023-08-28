#include <Core/include/VKLogicalDevice.h>
#include <Core/include/VKObjectWrapper.h>
#include <Core/include/VKPhysicalDevice.h>
#include <Core/Types.h>
#include <Debug/VKDebug.h>
#include <PotatoEngine/Util/Assert.h>
#include <stdexcept>

using namespace potato;
using namespace potato::vk;


// TODO: Use VK Allocator

VKLogicalDevice::VKLogicalDevice(const VKPhysicalDevice& physicalDevice, const VkDeviceCreateInfo& deviceCreateInfo, const VKCreateInfo& createInfo)
{
    if (vkCreateDevice(physicalDevice.getVkPhysicalDevice(), &deviceCreateInfo, nullptr, &m_device) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create logical device!");
    }

    m_enabledShaderStages = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
    if (deviceCreateInfo.pEnabledFeatures->geometryShader)
        m_enabledShaderStages |= VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
    if (deviceCreateInfo.pEnabledFeatures->tessellationShader)
        m_enabledShaderStages |= VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT | VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
// TODO:
//    if (m_enabledExtFeatures.MeshShader.meshShader != VK_FALSE && m_EnabledExtFeatures.MeshShader.taskShader != VK_FALSE)
//        m_enabledShaderStages |= VK_PIPELINE_STAGE_TASK_SHADER_BIT_NV | VK_PIPELINE_STAGE_MESH_SHADER_BIT_NV;
//    if (m_EnabledExtFeatures.RayTracingPipeline.rayTracingPipeline != VK_FALSE)
//        m_enabledShaderStages |= VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR;
}

VKLogicalDevice::~VKLogicalDevice()
{

}

template <typename VkObjectType, VKHandleTypeId VkTypeId, typename VkCreateObjectFuncType, typename VkObjectCreateInfoType>
VKObjectWrapper<VkObjectType, VkTypeId> VKLogicalDevice::createVulkanObject(
    VkCreateObjectFuncType VkCreateObject,
    const VkObjectCreateInfoType& createInfo,
    const char* debugName,
    const char* objectType) const
{
    if (debugName == nullptr)
        debugName = "";

    VkObjectType VkObject = VK_NULL_HANDLE;

    auto err = VkCreateObject(m_device, &createInfo, VK_NULL_HANDLE, &VkObject);
    POTATO_ASSERT_MSG(err == VK_SUCCESS, "Failed to create Vulkan %s '%s'", objectType, debugName);

    if (*debugName != 0)
        setVulkanObjectName<VkObjectType, VkTypeId>(m_device, VkObject, debugName);

    return VKObjectWrapper<VkObjectType, VkTypeId>{*this, std::move(VkObject)};
}

VkQueue VKLogicalDevice::getQueue(uint32_t queueFamilyIndex, uint32_t queueIndex) const
{
    VkQueue vkQueue = VK_NULL_HANDLE;
    vkGetDeviceQueue(m_device,
        queueFamilyIndex, // Index of the queue family to which the queue belongs
        0,                // Index within this queue family of the queue to retrieve
        &vkQueue);
    POTATO_ASSERT_MSG(vkQueue != VK_NULL_HANDLE, "Couldn't find a VKQueue with that combination of 'queueFamilyIndex' + 'queueIndex'");
    return vkQueue;
}

CommandPoolWrapper VKLogicalDevice::createCommandPool(const VkCommandPoolCreateInfo& cmdPoolCI, const char* debugName) const
{
    POTATO_ASSERT(cmdPoolCI.sType == VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO);
    return createVulkanObject<VkCommandPool, VKHandleTypeId::CommandPool>(vkCreateCommandPool, cmdPoolCI, debugName, "command pool");
}

FenceWrapper VKLogicalDevice::createFence(const VkFenceCreateInfo& fenceCI, const char* debugName) const
{
    POTATO_ASSERT(fenceCI.sType == VK_STRUCTURE_TYPE_FENCE_CREATE_INFO);
    return createVulkanObject<VkFence, VKHandleTypeId::Fence>(vkCreateFence, fenceCI, debugName, "fence");
}

ImageViewWrapper VKLogicalDevice::createImageView(const VkImageViewCreateInfo& imageViewCI, const char* debugName) const
{
    POTATO_ASSERT(imageViewCI.sType == VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO);
    return createVulkanObject<VkImageView, VKHandleTypeId::ImageView>(vkCreateImageView, imageViewCI, debugName, "image view");
}

RenderPassWrapper VKLogicalDevice::createRenderPass(const VkRenderPassCreateInfo& renderPassCI, const char* debugName = "") const
{
    POTATO_ASSERT(renderPassCI.sType == VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO);
    return createVulkanObject<VkRenderPass, VKHandleTypeId::RenderPass>(vkCreateRenderPass, renderPassCI, debugName, "render pass");
}

ShaderModuleWrapper VKLogicalDevice::createShaderModule(const VkShaderModuleCreateInfo& shaderModuleCI, const char* debugName = "") const
{
    POTATO_ASSERT(shaderModuleCI.sType == VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO);
    return createVulkanObject<VkShaderModule, VKHandleTypeId::ShaderModule>(vkCreateShaderModule, shaderModuleCI, debugName, "shader module");
}


void VKLogicalDevice::releaseVulkanObject(CommandPoolWrapper&& cmdPool) const
{
    vkDestroyCommandPool(m_device, cmdPool.m_VKObject, VK_NULL_HANDLE);
    cmdPool.m_VKObject = VK_NULL_HANDLE;
}

void VKLogicalDevice::releaseVulkanObject(FenceWrapper&& fence) const
{
    vkDestroyFence(m_device, fence.m_VKObject, VK_NULL_HANDLE);
    fence.m_VKObject = VK_NULL_HANDLE;
}

void VKLogicalDevice::releaseVulkanObject(ImageViewWrapper&& imageView) const
{
    vkDestroyImageView(m_device, imageView.m_VKObject, VK_NULL_HANDLE);
    imageView.m_VKObject = VK_NULL_HANDLE;
}

void VKLogicalDevice::releaseVulkanObject(RenderPassWrapper&& renderPass) const
{
    vkDestroyRenderPass(m_device, renderPass.m_VKObject, VK_NULL_HANDLE);
    renderPass.m_VKObject = VK_NULL_HANDLE;
}

void VKLogicalDevice::releaseVulkanObject(ShaderModuleWrapper&& shaderModule) const
{
    vkDestroyShaderModule(m_device, shaderModule.m_VKObject, VK_NULL_HANDLE);
    shaderModule.m_VKObject = VK_NULL_HANDLE;
}


VkResult VKLogicalDevice::getFenceStatus(VkFence fence) const
{
    return vkGetFenceStatus(m_device, fence);
}

VkResult VKLogicalDevice::resetFence(VkFence fence) const
{
    const auto err = vkResetFences(m_device, 1, &fence);
    POTATO_ASSERT_MSG(err == VK_SUCCESS, "vkResetFences() failed");
    return err;
}

VkResult VKLogicalDevice::waitForFences(uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) const
{
    return vkWaitForFences(m_device, fenceCount, pFences, waitAll, timeout);
}