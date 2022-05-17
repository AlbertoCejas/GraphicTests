#pragma once

#include <string>

#include <nlohmann/json_fwd.hpp>
#include <PotatoEngine/Core/Engine.h>
#include <PotatoEngine/PotatoEngineExport.h>

namespace potato
{
	class POTATOENGINE_EXPORT Application
	{
	public:

		explicit Application(const char* name);
		virtual ~Application() = default;

		void init(const char* configFilePath = "application.config");
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