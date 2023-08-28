#include <Core/include/VKInstance.h>
#include <PotatoEngine/Util/Assert.h>
#include <Core/Types.h>
#include <Debug/VKDebug.h>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>

using namespace potato;
using namespace potato::vk;


VKInstance::VKInstance(const VKCreateInfo& createInfo)
{
    if (createInfo.enableValidation && !checkValidationLayerSupport(createInfo.validationLayerNames))
    {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    {
        // Enumerate available extensions
        uint32_t extensionCount = 0;

        auto res = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        POTATO_ASSERT_MSG(res, "Failed to query extension count");
        m_extensions.resize(extensionCount);
        res = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, m_extensions.data());
        POTATO_ASSERT_MSG(res, "Failed to enumerate extensions");
        POTATO_ASSERT(extensionCount == m_extensions.size());
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = createInfo.appName;
    appInfo.applicationVersion = VK_MAKE_API_VERSION(createInfo.appVersion[0], createInfo.appVersion[1], createInfo.appVersion[2], createInfo.appVersion[3]);
    appInfo.pEngineName = createInfo.engineName;
    appInfo.engineVersion = VK_MAKE_API_VERSION(createInfo.engineVersion[0], createInfo.engineVersion[1], createInfo.engineVersion[2], createInfo.engineVersion[3]);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo vkCreateInfo{};
    vkCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    vkCreateInfo.pApplicationInfo = &appInfo;

    std::vector<const char*> requiredExtensions;


    if (createInfo.enableValidation)
    {
        vkCreateInfo.enabledLayerCount = static_cast<uint32_t>(createInfo.validationLayerNames.size());
        vkCreateInfo.ppEnabledLayerNames = createInfo.validationLayerNames.data();

        m_debugUtilsEnabled = isExtensionAvailable(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        if (m_debugUtilsEnabled)
        {
            requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
    }
    else
    {
        vkCreateInfo.enabledLayerCount = 0;
        vkCreateInfo.pNext = nullptr;
    }

    vkCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
    vkCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();

    uint32_t VkextensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &VkextensionCount, nullptr);
    std::vector<VkExtensionProperties> supportedExtensions(VkextensionCount);
    if (vkEnumerateInstanceExtensionProperties(nullptr, &VkextensionCount, supportedExtensions.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to enumerate extension properties!");
    }
    for (uint32_t i = 0; i < vkCreateInfo.enabledExtensionCount; i++)
    {
        const char* const extension = requiredExtensions[i];
        const auto it = std::find_if(supportedExtensions.begin(), supportedExtensions.end(), [extension](const VkExtensionProperties& vkExtensionProperties) { return strcmp(extension, vkExtensionProperties.extensionName) == 0; });
        if (it == supportedExtensions.end())
        {
            throw std::runtime_error("A required extension is not supported");
        }
    }

    if (vkCreateInstance(&vkCreateInfo, nullptr, &m_instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create instance!");
    }

    // If requested, we enable the default validation layers for debugging
    if (m_debugUtilsEnabled)
    {
        const VkDebugUtilsMessageSeverityFlagsEXT messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        const VkDebugUtilsMessageTypeFlagsEXT messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        setupDebugging(m_instance, messageSeverity, messageType, nullptr);
    }
}

VKInstance::~VKInstance()
{
    vkDestroyInstance(m_instance, nullptr);
}

bool VKInstance::checkValidationLayerSupport(const std::vector<const char*>& validationLayers) const
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers)
    {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            return false;
        }
    }

    return true;
}

std::vector<const char*> VKInstance::getRequiredExtensions(const VKCreateInfo& createInfo) const
{
    std::vector<const char*> extensions(createInfo.instanceExtensionNames.begin(), createInfo.instanceExtensionNames.end());

    if (createInfo.enableValidation)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{

    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

bool VKInstance::isDeviceSuitable(VkPhysicalDevice device) const
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamiliesProperties(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamiliesProperties.data());

    // If an implementation exposes any queue family that supports graphics operations,
    // at least one queue family of at least one physical device exposed by the implementation
    // must support both graphics and compute operations.
    for (const auto& queueFamilyProperties: queueFamiliesProperties)
    {
        if ((queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0 &&
            (queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT) != 0)
        {
            return true;
        }
    }
    return false;
}

bool VKInstance::checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& deviceExtensions) const
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

VkPhysicalDevice VKInstance::pickPhysicalDevice(const std::vector<const char*>& deviceExtensions) const
{
    uint32_t deviceCount = 0;
    if (vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr) != VK_SUCCESS || deviceCount == 0)
    {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

    VkPhysicalDevice selectedPhysicalDevice = VK_NULL_HANDLE;
    for (const auto& device : devices)
    {
        if (isDeviceSuitable(device) && checkDeviceExtensionSupport(device, deviceExtensions))
        {
            selectedPhysicalDevice = device;

            VkPhysicalDeviceProperties deviceProps;
            vkGetPhysicalDeviceProperties(device, &deviceProps);
            if (deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                break; // If dedicated GPU, let's go with it

            //m_msaaSamples = getMaxUsableSampleCount();
        }
    }

    if (selectedPhysicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("failed to find a suitable GPU!");
    }

    return selectedPhysicalDevice;
}

bool VKInstance::isExtensionAvailable(const char* ExtensionName) const
{
    for (const auto& Extension : m_extensions)
        if (strcmp(Extension.extensionName, ExtensionName) == 0)
            return true;

    return false;
}