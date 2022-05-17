#pragma once

#include <nlohmann/json_fwd.hpp>

#include <PotatoEngine/PotatoEngineExport.h>
#include <PotatoEngine/Util/Assert.h>

namespace potato
{
	class Application;

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


		explicit Plugin(int priority);
		virtual ~Plugin() = default;

		virtual const char* getName() const = 0;
		int getPriority() const { return m_priority; }
		Metadata& getMetadata() { return m_metadata; }

		virtual void init() = 0;
		virtual void init(const nlohmann::json& configJson) = 0;
		virtual void update() = 0;

		virtual void onLoaded(Application& app) {};

	protected:

		Metadata m_metadata;

	private:

		const int8_t m_priority;
	};
}