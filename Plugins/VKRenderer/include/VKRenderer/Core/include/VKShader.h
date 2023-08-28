#pragma once

#include <cstdint>
#include <Core/include/VKRenderDevice.h>
#include <Core/interface/VKIShader.h>
#include <PotatoEngine/Render/include/BaseShader.h>
#include <string>
#include <vector>


namespace potato
{
	namespace vk
	{
		class VKShader final : public BaseShader<VKIShader, VKRenderDevice>
		{
			public:

				using Base = BaseShader<VKIShader, VKRenderDevice>;

				VKShader(VKRenderDevice& renderDevice, const ShaderCreateInfo& shaderCI);
				~VKShader() override;

				const std::vector<uint32_t>& getSPIRV() const final { return m_SPIRV; }

				//const char* getEntryPoint() const { return m_entryPoint.c_str(); }

			private:

				//std::string m_entryPoint;
				std::vector<uint32_t> m_SPIRV;
		};
	}
}