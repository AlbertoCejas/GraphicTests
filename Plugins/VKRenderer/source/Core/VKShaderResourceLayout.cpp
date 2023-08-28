#include <Core/include/VKShaderResourceLayout.h>

#include <Core/include/VKShader.h>

using namespace potato;
using namespace potato::vk;


ShaderResourceLayoutVk::ShaderStageInfo::ShaderStageInfo(const VKShader* shader)
    : type{ shader->getDesc().shaderType }
    , shaders{ shader }
    , SPIRVs{ shader->getSPIRV() }
{
}

void ShaderResourceLayoutVk::ShaderStageInfo::append(const VKShader* shader)
{
    POTATO_ASSERT(shader != nullptr);
    POTATO_ASSERT_MSG(std::find(shaders.begin(), shaders.end(), shader) == shaders.end(), 
        "Shader already exists in the stage. Shaders must be deduplicated.");

    const auto newShaderType = shader->getDesc().shaderType;
    if (type == SHADER_TYPE_UNKNOWN)
    {
        POTATO_ASSERT(shaders.empty() && SPIRVs.empty());
        type = newShaderType;
    }
    else
    {
        POTATO_ASSERT(type == newShaderType, "The type of shader being added to the stage is incosistent with the stage type.");
    }

    shaders.push_back(shader);
    SPIRVs.push_back(shader->getSPIRV());
}

size_t ShaderResourceLayoutVk::ShaderStageInfo::count() const
{
    POTATO_ASSERT(shaders.size() == SPIRVs.size());
    return shaders.size();
}