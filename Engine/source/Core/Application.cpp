#include "Core/Application.h"

#include <nlohmann/json.hpp>
#include "File/File.h"
#include "File/FileSystem.h"
#include "Util/Assert.h"

namespace potato
{
	nlohmann::json parseConfig(const char* configFilePath);

	Application::Application(const char* name) :
		m_name(name),
		m_exit(false)
	{
		POTATO_ASSERT_MSG(name != nullptr, "Application name is nullptr");
	}

	void Application::init(const char* configFilePath)
	{
		nlohmann::json config = parseConfig(configFilePath);

		
		const auto& appJson = config["app"];
		POTATO_ASSERT_MSG(appJson.is_discarded() == false, "Config file doesn't contain 'app' section");

		// Set application name
		const auto& appNameJson = appJson["name"];
		POTATO_ASSERT_MSG(appNameJson.is_discarded() == false, "Config file doesn't contain app 'name' field");
		m_name = appNameJson.get<std::string>();

		m_engine.init(config);

		initImpl(config);
	}

	void Application::update()
	{
		m_engine.update();
		updateImpl();
	}

	void Application::shutdown()
	{
		m_engine.shutdown();
	}

	nlohmann::json parseConfig(const char* configFilePath)
	{
		std::unique_ptr<File> configFile = FileSystem::openFile(configFilePath, FileAccessMode::Read);
		POTATO_ASSERT_MSG(configFile != nullptr, "Config file '%s' couldn't be found", configFilePath);

		//size_t configFileSize = configFile->getSize();
		//std::string configFileStr;
		//configFileStr.reserve(configFileSize);
		//configFile->read(configFileStr.data(), configFileSize);

		//nlohmann::json configJson = nlohmann::json::parse(configFileStr);

		const nlohmann::json configJson = nlohmann::json::parse(configFile->getCFile(), nullptr, false);
		POTATO_ASSERT_MSG(configJson.is_discarded() == false, "Config file has invalid Json syntax");
		return configJson;
	}
}