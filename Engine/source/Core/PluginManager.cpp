#include "Core/PluginManager.h"

#include <filesystem>
#include <nlohmann/json.hpp>

#include "Core/Engine.h"
#include "Core/Plugin.h"
#include "Core/SharedLibrary.h"
#include "Util/Assert.h"

namespace potato
{
	bool PluginPriorityComparator::operator() (const Plugin* const lhs, const Plugin* const rhs) const
	{
		return lhs->getPriority() < rhs->getPriority();
	}

	PluginManager::PluginManager(Engine& engine) : m_engine(engine) {}

	void PluginManager::init(const nlohmann::json& appJson, const nlohmann::json& engineJson, const nlohmann::json& pluginsJson)
	{
		std::vector<const nlohmann::json*> pluginConfigs;
		pluginConfigs.reserve(pluginsJson.size());

		for (const auto& pluginJson : pluginsJson)
		{
			const auto& pluginNameJson = pluginJson["name"];
			POTATO_ASSERT_MSG(pluginNameJson.is_discarded() == false, "Plugins must have a name.");

			const std::string pluginName = pluginNameJson.get<std::string>();

			const int pluginPriority = pluginJson.value("priority", 1);

			static constexpr const char* const PLUGINS_FOLDER = "../Plugins/";
			static constexpr const char* const BINARY_FOLDER = "/bin/";

			const std::string pluginLibraryPath = PLUGINS_FOLDER + pluginName + BINARY_FOLDER + pluginName;
			const Plugin* plugin = loadPlugin(pluginLibraryPath.c_str());
			if (plugin != nullptr)
			{
				pluginConfigs.push_back(&pluginJson);
			}
		}

		for (size_t i = 0; i < m_activePlugins.size(); i++)
		{
			m_activePlugins[i]->init(appJson, engineJson, *pluginConfigs[i]);
		}
	}

	void PluginManager::shutdown()
	{
		for (Plugin* plugin : m_activePlugins)
		{
			delete plugin;
		}

		m_activePlugins.clear();
	}


	Plugin* PluginManager::loadPlugin(const char* pluginPath)
	{
		Plugin* pluginPtr = nullptr;

		// Load shared library
		void* sharedLibHandle = SharedLibrary::load(pluginPath);

		// Get plugin name and accessor function
		using CreatePluginFunctionSignature = Plugin& (*)(Engine&);
		using GetPluginNameFunctionSignature = const char* (*)();

		static constexpr const char CREATE_FUNCTION_PREFIX[] = "createPlugin";
		static constexpr const char GET_NAME_FUNCTION_NAME[] = "getPluginName";

		GetPluginNameFunctionSignature getPluginNameFunction = SharedLibrary::getFunctionPtr<GetPluginNameFunctionSignature>(sharedLibHandle, GET_NAME_FUNCTION_NAME);
		const std::string pluginName = getPluginNameFunction();

		// Make sure it isn't duplicated
		for (const Plugin* const activePlugin : m_activePlugins)
		{
			if (strcmp(pluginName.c_str(), activePlugin->getName()) == 0)
			{
				SharedLibrary::unload(pluginPath, sharedLibHandle);
				return nullptr;
			}
		}

		// Create and register plugin into PluginManager
		const std::string createFunctionName = CREATE_FUNCTION_PREFIX + pluginName;
		CreatePluginFunctionSignature createPluginFunction = SharedLibrary::getFunctionPtr<CreatePluginFunctionSignature>(sharedLibHandle, createFunctionName.c_str());
		pluginPtr = &(createPluginFunction(m_engine));
		POTATO_ASSERT_MSG(pluginPtr != nullptr, "Plugin '%' was not properly created", pluginName.c_str());
		m_activePlugins.push(pluginPtr);
		
		// Update metadata
		Plugin::Metadata& pluginMetadata = pluginPtr->getMetadata();
		pluginMetadata.handle = sharedLibHandle;
		pluginMetadata.path = pluginPath;

		return pluginPtr;
	}

	void PluginManager::unloadPlugin(Plugin& plugin)
	{
		const Plugin::Metadata& pluginMetadata = plugin.getMetadata();
		
		m_activePlugins.erase(&plugin);

		if (pluginMetadata.handle != nullptr)
		{
			SharedLibrary::unload(pluginMetadata.path, pluginMetadata.handle);
		}
		else
		{
			delete& plugin;
		}
	}

}