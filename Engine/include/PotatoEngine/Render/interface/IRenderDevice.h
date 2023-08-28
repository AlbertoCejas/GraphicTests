#pragma once

namespace potato
{
	class IRenderPass;
	class IShader;

	class IRenderDevice
	{
		public:

			virtual ~IRenderDevice() {}

			virtual IRenderPass* createRenderPass(const RenderPassDesc& desc) = 0;
			virtual IShader* createShader(const ShaderCreateInfo& shaderCI) = 0;
	};
}