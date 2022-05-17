#include "VkRendererExport.h"
#include <PotatoEngine/Core/Plugin.h>
#include <PotatoEngine/Util/Assert.h>

namespace potato
{
	class VkRendererPlugin : public Plugin
	{
	public:

		constexpr static const char* const NAME = "VkRenderer";

		explicit VkRendererPlugin(int priority);

		const char* getName() const override { return NAME; }

		void init() override;
		void init(const nlohmann::json& configJson) override;
		void update() override;
	};

	VkRendererPlugin::VkRendererPlugin(int priority) :
		Plugin(priority)
	{
	}

	void VkRendererPlugin::init()
	{

	}

	void VkRendererPlugin::init(const nlohmann::json& configJson)
	{

	}

	void VkRendererPlugin::update()
	{

	}
}

potato::Plugin* VkRenderer = nullptr;


extern "C" VKRENDERER_EXPORT const char* getPluginName()
{
	return potato::VkRendererPlugin::NAME;
}

extern "C" VKRENDERER_EXPORT potato::Plugin& createPluginVkRenderer()
{
	POTATO_ASSERT_MSG(VkRenderer == nullptr, "VkRenderer is already initialized");
	VkRenderer = new potato::VkRendererPlugin(0);
	return *VkRenderer;
}

extern "C" VKRENDERER_EXPORT void destroyPluginVkRenderer()
{
	delete VkRenderer;
}