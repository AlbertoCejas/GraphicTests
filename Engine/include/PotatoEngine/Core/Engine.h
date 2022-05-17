#pragma once

#include <nlohmann/json_fwd.hpp>

#include <PotatoEngine/PotatoEngineExport.h>

namespace potato
{
	class PluginManager;

	class POTATOENGINE_EXPORT Engine
	{
	public:
		
		void init();
		void init(const nlohmann::json& configJson);
		void shutdown();

		void update();

		PluginManager* getPluginManager() { return m_pluginManager; }

	private:
		PluginManager* m_pluginManager = nullptr;
	};
}