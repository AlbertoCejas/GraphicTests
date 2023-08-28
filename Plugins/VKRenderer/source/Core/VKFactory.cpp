#include <Core/include/VKFactory.h>

#include <array>
#include <Core/include/VKCommandQueue.h>
#include <Core/include/VKFence.h>
#include <Core/include/VKInstance.h>
#include <Core/include/VKLogicalDevice.h>
#include <Core/include/VKPhysicalDevice.h>
#include <Core/include/VKRenderContext.h>
#include <Core/include/VKRenderDevice.h>
#include <Core/include/VKSwapChain.h>
#include <Core/Types.h>
#include <iostream>
#include <PotatoEngine/Util/Assert.h>
#include <PotatoEngine/Render/include/GraphicsTypes.h>
#include <PotatoEngine/Render/interface/IFence.h>
#include <set>
#include <stdexcept>
#include <string>

using namespace potato;
using namespace potato::vk;

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{

    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

VKFactory::~VKFactory()
{

}

VKRenderDevice* VKFactory::createRenderDevice(VKCreateInfo& createInfo)
{
    // TODO: find a better home for this
    createInfo.validationLayerNames.push_back("VK_LAYER_KHRONOS_validation");

    VKInstance* instance = new VKInstance(createInfo);
    //setupDebugMessenger(createInfo);
    fillRequiredDeviceExtensions(createInfo.deviceExtensionNames);
    VkPhysicalDevice vkPhysicalDevice = instance->pickPhysicalDevice(createInfo.deviceExtensionNames);
    VKPhysicalDevice* physicalDevice = new VKPhysicalDevice(vkPhysicalDevice);

    // If an implementation exposes any queue family that supports graphics operations,
    // at least one queue family of at least one physical device exposed by the implementation
    // must support both graphics and compute operations.
    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.flags = 0; // reserved for future use
    // All commands that are allowed on a queue that supports transfer operations are also allowed on a
    // queue that supports either graphics or compute operations. Thus, if the capabilities of a queue family
    // include VK_QUEUE_GRAPHICS_BIT or VK_QUEUE_COMPUTE_BIT, then reporting the VK_QUEUE_TRANSFER_BIT
    // capability separately for that queue family is optional (4.1).
    queueInfo.queueFamilyIndex = physicalDevice->findQueueFamily(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
    queueInfo.queueCount = 1;
    const float defaultQueuePriority = 1.0f; // Ask for highest priority for our queue. (range [0,1])
    queueInfo.pQueuePriorities = &defaultQueuePriority;

    // TODO: enable device features from config
    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_FALSE; //VK_TRUE;
    deviceFeatures.sampleRateShading = VK_FALSE; //VK_TRUE; // enable sample shading feature for the device

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueInfo;
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(createInfo.deviceExtensionNames.size());
    deviceCreateInfo.ppEnabledExtensionNames = createInfo.deviceExtensionNames.data();

    if (createInfo.enableValidation)
    {
        deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(createInfo.validationLayerNames.size());
        deviceCreateInfo.ppEnabledLayerNames = createInfo.validationLayerNames.data();
    }
    else
    {
        deviceCreateInfo.enabledLayerCount = 0;
    }

    VKLogicalDevice* logicalDevice = new VKLogicalDevice(*physicalDevice, deviceCreateInfo, createInfo);

    // TODO -> SetDebugMessageCallback

    VKCommandQueue* commandQueue = new VKCommandQueue(*logicalDevice, queueInfo.queueFamilyIndex);
    std::array<VKICommandQueue*, 1> commandQueues = { { commandQueue } };
    VKRenderDevice* renderDevice = new VKRenderDevice(instance, physicalDevice, logicalDevice, commandQueues.size(), commandQueues.data());

    FenceDesc fenceDesc;
    fenceDesc.name = "Command queue internal fence";
    // Render device owns command queue that in turn owns the fence, so it is an internal device object
    constexpr bool IsDeviceInternal = true;
    VKFence* fenceVkPtr = new VKFence(*renderDevice, fenceDesc, IsDeviceInternal);
    commandQueue->setFence(*fenceVkPtr);

    return renderDevice;
}

void VKFactory::fillRequiredDeviceExtensions(std::vector<const char*>& deviceExtensions) const
{
    //TODO: check it doesn't exist
    deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

VKRenderContext* VKFactory::createRenderContexts(VKRenderDevice& renderDevice)
{
    VKRenderContext* immediateRenderContext = new VKRenderContext(renderDevice, 0, 0);
    renderDevice.setImmediateContext(*immediateRenderContext);
    // TODO: deferred render contexts?
    return immediateRenderContext;
}

VKSwapChain* VKFactory::createSwapchain(VKRenderDevice& renderDevice, VKRenderContext& context, const SwapChainDesc& swapChainDesc, Window& window)
{
    return new VKSwapChain(renderDevice, context, swapChainDesc, window);
}

