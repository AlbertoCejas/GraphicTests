#include "Core/Engine.h"

#include <nlohmann/json.hpp>

#include "Core/PluginManager.h"
#include "Core/Plugin.h"
#include "Core/WindowManager.h"
#include "Util/Assert.h"

namespace potato
{
	void Engine::init(const nlohmann::json& configJson)
	{
		// TODO: surround with ifdef "!HEADLESS_MODE"
		POTATO_FATAL_ASSERT_MSG(m_windowManager == nullptr, "WindowManager is already initialized");
		m_windowManager = new WindowManager();

		const auto& appJson = configJson["app"];
		POTATO_ASSERT_MSG(appJson.is_discarded() == false, "Config file doesn't contain 'app' section");
		const auto& engineJson = configJson["engine"];
		POTATO_ASSERT_MSG(engineJson.is_discarded() == false, "Config file doesn't contain 'engine' section");
		const auto& pluginsJson = configJson["plugins"];
		POTATO_ASSERT_MSG(pluginsJson.is_discarded() == false, "Config file doesn't contain 'plugins' section");

		POTATO_FATAL_ASSERT_MSG(m_pluginManager == nullptr, "PluginManager is already initialized");
		m_pluginManager = new PluginManager(*this);
		m_pluginManager->init(appJson, engineJson, pluginsJson);
	}

	void Engine::shutdown()
	{
		m_pluginManager->shutdown();
		delete m_pluginManager;
		m_pluginManager = nullptr;

		delete m_windowManager;
		m_windowManager = nullptr;
	}

	void Engine::update()
	{
		for (Plugin* plugin : m_pluginManager->getActivePlugins())
		{
			plugin->update();
		}
	}


}