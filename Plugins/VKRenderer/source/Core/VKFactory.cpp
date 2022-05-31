#include <Core/include/VKFactory.h>
#include <Core/include/VKRenderContext.h>
#include <Core/include/VKRenderDevice.h>
#include <Core/include/VKSwapChain.h>
#include <Core/Types.h>
#include <iostream>
#include <PotatoEngine/Util/Assert.h>
#include <PotatoEngine/Render/include/GraphicsTypes.h>
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
    delete m_renderContext;
    delete m_renderDevice;
}

void VKFactory::init(VKCreateInfo& createInfo)
{
    createInstance(createInfo);
    setupDebugMessenger(createInfo);
    fillRequiredDeviceExtensions(createInfo.deviceExtensionNames);
    pickPhysicalDevice(createInfo.deviceExtensionNames);
    createLogicalDevice(createInfo);
    createRenderDevice(createInfo);
    createRenderContext(createInfo);
}

void VKFactory::createInstance(const VKCreateInfo& createInfo)
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

    const auto requiredExtensions = getRequiredExtensions(createInfo);
    vkCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
    vkCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (createInfo.enableValidation)
    {
        vkCreateInfo.enabledLayerCount = static_cast<uint32_t>(createInfo.validationLayerNames.size());
        vkCreateInfo.ppEnabledLayerNames = createInfo.validationLayerNames.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        debugCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
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
}

bool VKFactory::checkValidationLayerSupport(const std::vector<const char*>& validationLayers) const
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

std::vector<const char*> VKFactory::getRequiredExtensions(const VKCreateInfo& createInfo) const
{
    std::vector<const char*> extensions(createInfo.instanceExtensionNames.begin(), createInfo.instanceExtensionNames.end());

    if (createInfo.enableValidation)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}


void VKFactory::setupDebugMessenger(const VKCreateInfo& engineVKcreateInfo)
{
    if (!engineVKcreateInfo.enableValidation)
    {
        return;
    }

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    populateDebugMessengerCreateInfo(createInfo);

    if (createDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

void VKFactory::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) const
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr; // Optional
}

VkResult VKFactory::createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) const
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void VKFactory::fillRequiredDeviceExtensions(std::vector<const char*>& deviceExtensions) const
{
    //TODO: check it doesn't exist
    deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

void VKFactory::pickPhysicalDevice(const std::vector<const char*>& deviceExtensions)
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
        if (isDeviceSuitable(device, deviceExtensions))
        {
            m_physicalDevice = device;

            VkPhysicalDeviceProperties deviceProps;
            vkGetPhysicalDeviceProperties(device, &deviceProps);
            if (deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                return; // If dedicated GPU, let's go for it

            //m_msaaSamples = getMaxUsableSampleCount();
        }
    }

    if (m_physicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

bool VKFactory::isDeviceSuitable(VkPhysicalDevice device, const std::vector<const char*>& deviceExtensions) const
{
    const QueueFamilyIndices indices = findQueueFamilies(device);

    const bool extensionsSupported = checkDeviceExtensionSupport(device, deviceExtensions);

    return indices.isComplete() && extensionsSupported;
}


bool VKFactory::checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& deviceExtensions) const
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

void VKFactory::createLogicalDevice(const VKCreateInfo& engineVKcreateInfo)
{
    // TODO: comparar findQueueFamiliy con VulkanPhysicalDevice -> FIndQueueFamily
    const QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.computeFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_FALSE; //VK_TRUE;
    deviceFeatures.sampleRateShading = VK_FALSE; //VK_TRUE; // enable sample shading feature for the device

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(engineVKcreateInfo.deviceExtensionNames.size());
    createInfo.ppEnabledExtensionNames = engineVKcreateInfo.deviceExtensionNames.data();

    if (engineVKcreateInfo.enableValidation)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(engineVKcreateInfo.validationLayerNames.size());
        createInfo.ppEnabledLayerNames = engineVKcreateInfo.validationLayerNames.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }
    if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_logicalDevice) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(m_logicalDevice, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_logicalDevice, indices.computeFamily.value(), 0, &m_computeQueue);
}

void VKFactory::createRenderDevice(const VKCreateInfo& createInfo)
{
    m_renderDevice = new VKRenderDevice(m_instance);
}

void VKFactory::createRenderContext(const VKCreateInfo& createInfo)
{
    m_renderContext = new VKRenderContext();
}

void VKFactory::createSwapchain(IRenderDevice& renderDevice, IRenderContext& context, const SwapChainDesc& swapChainDesc, Window& window, ISwapChain* swapChain)
{
    POTATO_ASSERT_MSG(swapChain == nullptr, "Provided swapchain is already initialized");
    swapChain = new VKSwapChain(renderDevice, window);
    /*
    
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan window", nullptr, nullptr);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
    
    */
}


QueueFamilyIndices VKFactory::findQueueFamilies(VkPhysicalDevice device) const
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            indices.computeFamily = i;
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


