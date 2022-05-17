#include "GLRendererExport.h"
#include <PotatoEngine/Core/Plugin.h>
#include <PotatoEngine/Util/Assert.h>

namespace potato
{
	class GLRendererPlugin : public Plugin
	{
	public:

		constexpr static const char* const NAME = "GLRenderer";

		explicit GLRendererPlugin(int priority);

		const char* getName() const override { return NAME; }

		void init() override;
		void init(const nlohmann::json& configJson) override;
		void update() override;
	};

	GLRendererPlugin::GLRendererPlugin(int priority) :
		Plugin(priority)
	{
	}

	void GLRendererPlugin::init()
	{

	}

	void GLRendererPlugin::init(const nlohmann::json& configJson)
	{

	}

	void GLRendererPlugin::update()
	{

	}
}

potato::Plugin* glRenderer = nullptr;


extern "C" GLRENDERER_EXPORT const char* getPluginName()
{
	return potato::GLRendererPlugin::NAME;
}

extern "C" GLRENDERER_EXPORT potato::Plugin& createPluginGLRenderer()
{
	POTATO_ASSERT_MSG(glRenderer == nullptr, "GLRenderer is already initialized");
	glRenderer = new potato::GLRendererPlugin(0);
	return *glRenderer;
}

extern "C" GLRENDERER_EXPORT void destroyPluginGLRenderer()
{
	delete glRenderer;
}