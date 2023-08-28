#pragma once

#include <nlohmann/json_fwd.hpp>

#include <PotatoEngine/PotatoEngineExport.h>
#include <PotatoEngine/Util/Assert.h>

namespace potato
{
	class Engine;

	// TODO: Add enum class bit flag capabilities
	enum /*class*/ PluginType : uint8_t
	{
		Renderer = 0x00
	};

	class POTATOENGINE_EXPORT Plugin
	{
		public:

			struct POTATOENGINE_EXPORT Metadata
			{
				void* handle = nullptr;
				const char* path = nullptr;
				const char* author = nullptr;
				// category
			};


			explicit Plugin(Engine& engine, int priority);
			virtual ~Plugin() = default;

			virtual const char* getName() const = 0;
			virtual PluginType getType() const = 0;
			int getPriority() const { return m_priority; }
			Metadata& getMetadata() { return m_metadata; }

			virtual void init(const nlohmann::json& appConfigJson, const nlohmann::json& engineConfigJson, const nlohmann::json& pluginConfigJson) = 0;
			virtual void update() = 0;

		protected:

			Metadata m_metadata;
			Engine& m_engine;
			const int8_t m_priority;
	};
}