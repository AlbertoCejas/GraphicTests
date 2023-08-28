#include <Debug/VKDebug.h>

#include <Core/include/VKLogicalDevice.h>
#include <iostream>
#include <PotatoEngine/Util/Assert.h>

using namespace potato;
using namespace potato::vk;

static PFN_vkCreateDebugUtilsMessengerEXT  g_createDebugUtilsMessengerEXT = nullptr;
static PFN_vkDestroyDebugUtilsMessengerEXT g_destroyDebugUtilsMessengerEXT = nullptr;
static PFN_vkSetDebugUtilsObjectNameEXT g_setDebugUtilsObjectNameEXT = nullptr;

static VkDebugUtilsMessengerEXT g_dbgMessenger = VK_NULL_HANDLE;

VKAPI_ATTR VkBool32 VKAPI_CALL debugMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
    void* userData)
{
    // TODO: improve this function
    std::cerr << "validation layer: " << callbackData->pMessage << std::endl;
    return VK_FALSE;
}

void potato::vk::setupDebugging(VkInstance instance, VkDebugUtilsMessageSeverityFlagsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, void* pUserData = nullptr)
{
    g_createDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
    g_destroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
    POTATO_ASSERT(g_createDebugUtilsMessengerEXT != nullptr && g_destroyDebugUtilsMessengerEXT != nullptr);

    VkDebugUtilsMessengerCreateInfoEXT dbgMessenger_CI = {};

    dbgMessenger_CI.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    dbgMessenger_CI.pNext = NULL;
    dbgMessenger_CI.flags = 0;
    dbgMessenger_CI.messageSeverity = messageSeverity;
    dbgMessenger_CI.messageType = messageType;
    dbgMessenger_CI.pfnUserCallback = debugMessengerCallback;
    dbgMessenger_CI.pUserData = pUserData;

    VkResult err = g_createDebugUtilsMessengerEXT(instance, &dbgMessenger_CI, nullptr, &g_dbgMessenger);
    POTATO_ASSERT_MSG(err == VK_SUCCESS, "Failed to create debug utils messenger");
    (void)err;

    // Load function pointers
    g_setDebugUtilsObjectNameEXT = reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>(vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT"));
    POTATO_ASSERT(g_setDebugUtilsObjectNameEXT != nullptr);
}

void potato::vk::freeDebugging(VkInstance instance)
{
    if (g_dbgMessenger != VK_NULL_HANDLE)
    {
        g_destroyDebugUtilsMessengerEXT(instance, g_dbgMessenger, nullptr);
    }
}

void potato::vk::setObjectName(VkDevice device, uint64_t objectHandle, VkObjectType objectType, const char* name)
{
    // Check for valid function pointer (may not be present if not running in a debugging application)
    if (g_setDebugUtilsObjectNameEXT != nullptr && name != nullptr && *name != 0)
    {
        VkDebugUtilsObjectNameInfoEXT objectNameInfo = {};

        objectNameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
        objectNameInfo.pNext = nullptr;
        objectNameInfo.objectType = objectType;
        objectNameInfo.objectHandle = objectHandle;
        objectNameInfo.pObjectName = name;

        VkResult res = g_setDebugUtilsObjectNameEXT(device, &objectNameInfo);
        POTATO_ASSERT(res == VK_SUCCESS);
        POTATO_UNUSED(res);
    }
}

void potato::vk::setFenceName(VkDevice device, VkFence fence, const char* name)
{
    potato::vk::setObjectName(device, (uint64_t)fence, VK_OBJECT_TYPE_FENCE, name);
}

template <>
void potato::vk::setVulkanObjectName<VkFence, VKHandleTypeId::Fence>(VkDevice device, VkFence fence, const char* name)
{
    setFenceName(device, fence, name);
}