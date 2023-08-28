#pragma once

#include <PotatoEngine/Core/Plugin.h>

namespace potato
{
	class IRenderDevice;
	class ISwapChain;

	class POTATOENGINE_EXPORT RendererPlugin : public Plugin
	{
		public:

			using Plugin::Plugin; // Inherit constructors

			PluginType getType() const override { return PluginType::Renderer; }

			virtual IRenderDevice& getRenderDevice() const = 0;
			virtual ISwapChain& getSwapChain() const = 0;
	};
}