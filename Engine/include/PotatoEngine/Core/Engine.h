#pragma once

#include <nlohmann/json_fwd.hpp>

#include <PotatoEngine/PotatoEngineExport.h>

namespace potato
{
	class PluginManager;
	class WindowManager;

	class POTATOENGINE_EXPORT Engine
	{
	public:
		
		void init(const nlohmann::json& configJson);
		void shutdown();

		void update();

		PluginManager* getPluginManager() const { return m_pluginManager; }
		WindowManager* getWindowManager() const { return m_windowManager; }

	private:

		PluginManager* m_pluginManager = nullptr;
		WindowManager* m_windowManager = nullptr;
	};
}