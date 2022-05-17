#include <Core/VKFactory.h>

#include <Core/Types.h>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <vulkan/vulkan_core.h>

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

class VKFactoryImpl final : public VKFactory
{
	public:

	private:

		VkInstance createInstance(const VKCreateInfo& createInfo);
        bool checkValidationLayerSupport(const std::vector<const char*>& validationLayerNames);

        void pickPhysicalDevice();
        bool isDeviceSuitable(VkPhysicalDevice device);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);


        VkInstance m_instance = VK_NULL_HANDLE;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkDevice m_logicalDevice = VK_NULL_HANDLE;
        VkQueue m_graphicsQueue = VK_NULL_HANDLE;
        VkQueue m_presentQueue = VK_NULL_HANDLE;

        VkDebugUtilsMessengerEXT m_debugMessenger;
};

VkInstance VKFactoryImpl::createInstance(const VKCreateInfo& createInfo)
{
    if (createInfo.enableValidation && !checkValidationLayerSupport(createInfo.validationLayerNames))
    {
        throw std::runtime_error("validation layers requested, but not available!");
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

    vkCreateInfo.enabledExtensionCount = static_cast<uint32_t>(createInfo.extensionNames.size());
    vkCreateInfo.ppEnabledExtensionNames = createInfo.extensionNames.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (createInfo.enableValidation)
    {
        vkCreateInfo.enabledLayerCount = static_cast<uint32_t>(createInfo.validationLayerNames.size());
        vkCreateInfo.ppEnabledLayerNames = createInfo.validationLayerNames.data();

        debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugCreateInfo.pfnUserCallback = debugCallback;

        vkCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else
    {
        vkCreateInfo.enabledLayerCount = 0;
        vkCreateInfo.pNext = nullptr;
    }

    uint32_t VkextensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &VkextensionCount, nullptr);
    std::vector<VkExtensionProperties> supportedExtensions(VkextensionCount);
    if (vkEnumerateInstanceExtensionProperties(nullptr, &VkextensionCount, supportedExtensions.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to enumerate extension properties!");
    }
    for (uint32_t i = 0; i < vkCreateInfo.enabledExtensionCount; i++)
    {
        const char* const extension = createInfo.extensionNames[i];
        const auto it = std::find_if(supportedExtensions.begin(), supportedExtensions.end(), [extension](const VkExtensionProperties& vkExtensionProperties) { return strcmp(extension, vkExtensionProperties.extensionName) == 0; });
        if (it == supportedExtensions.end())
        {
            throw std::runtime_error("Glfw required extension is not supported");
        }
    }

    VkInstance instance;
    if (vkCreateInstance(&vkCreateInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create instance!");
    }

    // POTATO_BEGIN | TODO: this shouldn't be here

    VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo{};
    debugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugMessengerCreateInfo.pfnUserCallback = debugCallback;

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    bool debugMessegerCreated = false;
    if (func != nullptr)
    {
        if (func(instance, &debugMessengerCreateInfo, nullptr, &m_debugMessenger) == VK_SUCCESS)
        {
            debugMessegerCreated = true;
        }
    }
    if (debugMessegerCreated == false)
    {
        throw std::runtime_error("failed to set up debug messenger!");
    }

    // POTATO_END

    return instance;
}

bool VKFactoryImpl::checkValidationLayerSupport(const std::vector<const char*>& validationLayers)
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

void VKFactoryImpl::pickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    if (vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr) != VK_SUCCESS || deviceCount == 0)
    {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

    for (const auto& device : devices)
    {
        if (isDeviceSuitable(device))
        {
            m_physicalDevice = device;
            //m_msaaSamples = getMaxUsableSampleCount();
            break;
        }
    }

    if (m_physicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

bool VKFactoryImpl::isDeviceSuitable(VkPhysicalDevice device)
{
    const QueueFamilyIndices indices = findQueueFamilies(device);

    const bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

QueueFamilyIndices VKFactoryImpl::findQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        VkBool32 presentSupport = false;

        if (vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport) == VK_SUCCESS && presentSupport)
        {
            indices.presentFamily = i;
        }

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }
        if (indices.isComplete())
        {
            break;
        }
        i++;
    }

    return indices;
}

SwapChainSupportDetails VKFactoryImpl::querySwapChainSupport(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;

    if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to get physical device surface capabilities!");
    }

    uint32_t formatCount;
    if (vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to get physical device surface format!");
    }

    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    if (vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, nullptr) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to get physical device surface present modes count!");
    }

    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        if (vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, details.presentModes.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to get physical device surface present modes!");
        }
    }

    return details;
}

bool VKFactoryImpl::checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*> deviceExtensions)
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