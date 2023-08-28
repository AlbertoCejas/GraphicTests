#pragma once

#include <nlohmann/json_fwd.hpp>
#include <PotatoEngine/Collections/SortedVector.h>
#include <PotatoEngine/PotatoEngineExport.h>

namespace potato
{
	class Plugin;
	class Engine;

	struct POTATOENGINE_EXPORT PluginPriorityComparator
	{
		bool operator() (const Plugin* const lhs, const Plugin* const rhs) const;
	};

	class POTATOENGINE_EXPORT PluginManager
	{
	public:

		explicit PluginManager(Engine& engine);

		void init(const nlohmann::json& appJson, const nlohmann::json& engineJson, const nlohmann::json& pluginsJson);
		void shutdown();

		Plugin* loadPlugin(const char* pluginPath);
		void unloadPlugin(Plugin& plugin);

		const SortedVector<Plugin*, PluginPriorityComparator>& getActivePlugins() const { return m_activePlugins; }

	private:

		Engine& m_engine;
		SortedVector<Plugin*, PluginPriorityComparator> m_activePlugins;
	};
}