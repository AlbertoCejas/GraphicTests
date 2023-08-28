#pragma once

#include <string>

#include <nlohmann/json_fwd.hpp>
#include <PotatoEngine/Core/Engine.h>
#include <PotatoEngine/PotatoEngineExport.h>

namespace potato
{
	constexpr const char* const CONFIG_FILE_NAME = "application.config";

	class POTATOENGINE_EXPORT Application
	{
	public:

		explicit Application(const char* name);
		virtual ~Application() = default;

		void init(const char* configFilePath = CONFIG_FILE_NAME);
		void update();
		virtual void shutdown();

		bool shouldExit() const { return m_exit; }

	protected:

		virtual void initImpl(const nlohmann::json& configJson) {}
		virtual void updateImpl() = 0;

		std::string m_name;
		Engine m_engine;
		bool m_exit;
	};
}