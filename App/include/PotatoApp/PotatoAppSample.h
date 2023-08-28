#pragma once

#include <PotatoEngine/Core/Application.h>

namespace potato
{
	class RendererPlugin;
	class IRenderPass;
	class IPipelineState;
}

class PotatoAppSample final : public potato::Application
{
	public:

		// Inherit constructors
		using potato::Application::Application;

		void shutdown() final;

	private:

		void initImpl(const nlohmann::json& configJson) final;
		void updateImpl() final;

		void createRenderPass();
		void createGraphicsPipeline();

		RendererPlugin* m_renderer = nullptr;
		IRenderPass* m_renderPass = nullptr;
		IPipelineState* m_pipelineState = nullptr;
};