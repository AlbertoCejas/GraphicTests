#include "Core/Engine.h"

#include <nlohmann/json.hpp>

#include "Core/PluginManager.h"
#include "Core/Plugin.h"
#include "Util/Assert.h"

namespace potato
{
	void Engine::init()
	{
		POTATO_FATAL_ASSERT_MSG(m_pluginManager == nullptr, "Engine is already initialized");
		m_pluginManager = new PluginManager();
		m_pluginManager->init();
	}

	void Engine::init(const nlohmann::json& configJson)
	{
		POTATO_FATAL_ASSERT_MSG(m_pluginManager == nullptr, "Engine is already initialized");
		m_pluginManager = new PluginManager();

		const auto& pluginsJson = configJson["plugins"];
		POTATO_ASSERT_MSG(pluginsJson.is_discarded() == false, "Config file doesn't contain plugins field");
		m_pluginManager->init(pluginsJson);
	}

	void Engine::shutdown()
	{
		m_pluginManager->shutdown();
	}

	void Engine::update()
	{
		for (Plugin* plugin : m_pluginManager->getActivePlugins())
		{
			plugin->update();
		}
	}


}