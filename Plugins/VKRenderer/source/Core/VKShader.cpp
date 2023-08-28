#include <Core/include/VKShader.h>
#include <vulkan/vulkan_core.h>

using namespace potato;
using namespace potato::vk;


VKShader::VKShader(VKRenderDevice& renderDevice, const ShaderCreateInfo& shaderCI)
	: Base(renderDevice, shaderCI.desc)
{
	if (shaderCI.byteCode != nullptr)
	{
		POTATO_ASSERT_MSG(shaderCI.byteCodeSize != 0, "ByteCodeSize must not be 0");
		POTATO_ASSERT_MSG(shaderCI.byteCodeSize % 4 == 0, "Byte code size is not multiple of 4");
		m_SPIRV.resize(shaderCI.byteCodeSize / 4);
		memcpy(m_SPIRV.data(), shaderCI.byteCode, shaderCI.byteCodeSize);
	}
	// TODO: add support for source shader code and file
}

VKShader::~VKShader()
{

}
