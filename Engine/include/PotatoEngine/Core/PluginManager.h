#pragma once

#include <nlohmann/json_fwd.hpp>
#include <PotatoEngine/Collections/SortedVector.h>
#include <PotatoEngine/PotatoEngineExport.h>

namespace potato
{
	class Plugin;

	struct POTATOENGINE_EXPORT PluginPriorityComparator
	{
		bool operator() (const Plugin* const lhs, const Plugin* const rhs) const;
	};

	class POTATOENGINE_EXPORT PluginManager
	{
	public:

		void init();
		void init(const nlohmann::json& configJson);
		void shutdown();

		Plugin* loadPlugin(const char* pluginPath);
		void unloadPlugin(Plugin& plugin);

		const SortedVector<Plugin*, PluginPriorityComparator>& getActivePlugins() const { return m_activePlugins; }

	private:

		SortedVector<Plugin*, PluginPriorityComparator> m_activePlugins;
	};
}