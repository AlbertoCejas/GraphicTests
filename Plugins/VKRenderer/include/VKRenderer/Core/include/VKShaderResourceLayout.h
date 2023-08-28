#pragma once

#include <cstdint>
#include <PotatoEngine/Render/interface/IShader.h>
#include <vector>


namespace potato
{
	namespace vk
	{
        class VKShader;

		class ShaderResourceLayoutVk
		{
			public:

                struct ShaderStageInfo
                {
                    ShaderStageInfo() {}
                    ShaderStageInfo(const VKShader* shader);

                    void   append(const VKShader* shader);
                    size_t count() const;

                    // Shader stage type. All shaders in the stage must have the same type.
                    ShaderType type = SHADER_TYPE_UNKNOWN;

                    std::vector<const VKShader*> shaders;
                    std::vector<std::vector<uint32_t>> SPIRVs;
                };
                using TShaderStages = std::vector<ShaderStageInfo>;

            // TODO
		};
	}
}