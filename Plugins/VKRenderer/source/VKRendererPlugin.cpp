#include "VkRendererExport.h"
#include <Core/include/VKFactory.h>
#include <Core/Types.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <nlohmann/json.hpp>
#include <PotatoEngine/Core/Engine.h>
#include <PotatoEngine/Core/RendererPlugin.h>
#include <PotatoEngine/Core/WindowManager.h>
#include <PotatoEngine/Core/Window.h>
#include <PotatoEngine/Render/include/GraphicsTypes.h>
#include <Core/include/VKRenderDevice.h>
#include <Core/include/VKRenderContext.h>
#include <Core/include/VKSwapchain.h>
#include <PotatoEngine/Util/Assert.h>

namespace potato
{
	class VKRendererPlugin : public RendererPlugin
	{
		public:

			constexpr static const char* const NAME = "VKRenderer";

			explicit VKRendererPlugin(Engine& engine, int priority);
			~VKRendererPlugin() override;

			void init(const nlohmann::json& appConfigJson, const nlohmann::json& engineConfigJson, const nlohmann::json& pluginConfigJson) override;
			void update() override;

			const char* getName() const override { return NAME; }
			IRenderDevice& getRenderDevice() const override { return *m_renderDevice; }
			ISwapChain& getSwapChain() const override { return *m_swapChain; }

		private:

			vk::VKFactory* m_factory = nullptr;
			vk::VKRenderDevice* m_renderDevice = nullptr;
			vk::VKSwapChain* m_swapChain = nullptr;
	};

	VKRendererPlugin::VKRendererPlugin(Engine& engine, int priority) :
		RendererPlugin(engine, priority)
	{
	}

	VKRendererPlugin::~VKRendererPlugin()
	{
		if (m_renderDevice != nullptr)
		{
			delete m_renderDevice;
			m_renderDevice = nullptr;
		}

		if (m_factory != nullptr)
		{
			delete m_factory;
			m_factory = nullptr;
		}

		glfwTerminate();
	}

	void VKRendererPlugin::init(const nlohmann::json& appConfigJson, const nlohmann::json& engineConfigJson, const nlohmann::json& pluginConfigJson)
	{
		POTATO_ASSERT_MSG(m_factory == nullptr, "VKRendererPlugin has already been initialized");

		// Init GLFW. It has to be here since 'GLFW_INCLUDE_VULKAN' has to be defined
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		const auto& windowWidthJson = engineConfigJson["window_width"];
		const auto& windowHeightJson = engineConfigJson["window_height"];
		const int windowWidth = windowWidthJson.get<int>();
		const int windowHeight = windowHeightJson.get<int>();
		if (windowWidthJson.is_discarded() == false && windowHeightJson.is_discarded() == false)
		{
			const auto& appNameJson = appConfigJson["name"];
			POTATO_ASSERT_MSG(appNameJson.is_discarded() == false, "Config file doesn't contain app 'name' section");
			const auto appName = appNameJson.get<std::string>();
			WindowManager* windowManager = m_engine.getWindowManager();
			POTATO_ASSERT_MSG(windowManager != nullptr, "windowManager is nullptr");
			Window* window = windowManager->createWindow(appName.c_str(), windowWidth, windowHeight);
			windowManager->setPrimaryWindow(*window);
		}

		m_factory = new vk::VKFactory();

		// TODO: take data from json
		vk::VKCreateInfo createInfo =
		{
			.appName = "PotatoApp",
			.appVersion = {1, 0, 1, 0},
			.engineName = "Potato",
			.engineVersion = {1, 0, 1, 0},
			.enableValidation = false/*,
			std::vector<const char*> validationLayerNames;
			std::vector<const char*> instanceExtensionNames;
			std::vector<const char*> deviceExtensionNames;*/
		};



		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		if constexpr (std::is_same<NativeWindow, GLFWwindow>() == true)
		{
			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
			createInfo.instanceExtensionNames.assign(glfwExtensions, glfwExtensions + glfwExtensionCount);
		}

		m_renderDevice = m_factory->createRenderDevice(createInfo);
		m_factory->createRenderContexts(*m_renderDevice);
		// TODO: add ability to create deferred contexts

		SwapChainDesc swapChainDesc =
		{
			.width = windowWidth,
			.height = windowHeight
		};
		const WindowManager* windowManager = m_engine.getWindowManager();
		Window* primaryWindow = windowManager->getPrimaryWindow();

		IRenderContext* renderContext = m_renderDevice->getImmediateContext();
		m_swapChain = m_factory->createSwapchain(*m_renderDevice, static_cast<vk::VKRenderContext&>(*renderContext), swapChainDesc, *primaryWindow);
	}

	void VKRendererPlugin::update()
	{

	}
}

potato::Plugin* VkRenderer = nullptr;


extern "C" VKRENDERER_EXPORT const char* getPluginName()
{
	return potato::VKRendererPlugin::NAME;
}

extern "C" VKRENDERER_EXPORT potato::Plugin& createPluginVKRenderer(potato::Engine& engine)
{
	POTATO_ASSERT_MSG(VkRenderer == nullptr, "VkRenderer is already initialized");
	VkRenderer = new potato::VKRendererPlugin(engine, 0);
	return *VkRenderer;
}

extern "C" VKRENDERER_EXPORT void destroyPluginVkRenderer()
{
	delete VkRenderer;
}