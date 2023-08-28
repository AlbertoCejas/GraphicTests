#pragma once

#include <string>

namespace potato
{
	/// Describes the shader type
	enum ShaderType : uint32_t
	{
		SHADER_TYPE_UNKNOWN = 0x0000, ///< Unknown shader type
		SHADER_TYPE_VERTEX = 0x0001, ///< Vertex shader
		SHADER_TYPE_PIXEL = 0x0002, ///< Pixel (fragment) shader
		SHADER_TYPE_GEOMETRY = 0x0004, ///< Geometry shader
		SHADER_TYPE_HULL = 0x0008, ///< Hull (tessellation control) shader
		SHADER_TYPE_DOMAIN = 0x0010, ///< Domain (tessellation evaluation) shader
		SHADER_TYPE_COMPUTE = 0x0020, ///< Compute shader
		SHADER_TYPE_AMPLIFICATION = 0x0040, ///< Amplification (task) shader
		SHADER_TYPE_MESH = 0x0080, ///< Mesh shader
		SHADER_TYPE_RAY_GEN = 0x0100, ///< Ray generation shader
		SHADER_TYPE_RAY_MISS = 0x0200, ///< Ray miss shader
		SHADER_TYPE_RAY_CLOSEST_HIT = 0x0400, ///< Ray closest hit shader
		SHADER_TYPE_RAY_ANY_HIT = 0x0800, ///< Ray any hit shader
		SHADER_TYPE_RAY_INTERSECTION = 0x1000, ///< Ray intersection shader
		SHADER_TYPE_CALLABLE = 0x2000, ///< Callable shader
		SHADER_TYPE_LAST = SHADER_TYPE_CALLABLE
	};

	struct ShaderDesc
	{
		std::string name = nullptr;

		ShaderType shaderType = ShaderType::SHADER_TYPE_UNKNOWN;
	};

	/// Shader creation attributes
	struct ShaderCreateInfo
	{
		/// Shader description. See Diligent::ShaderDesc.
		ShaderDesc desc;

		/// byte code
		const void* byteCode = nullptr;
		/// Size of the compiled shader bytecode(in bytes) must be provided if byteCode is not null
		size_t byteCodeSize = 0;

		/// Shader entry point
		//const char* entryPoint = "main";
	};

	class IShader
	{
		public:

			virtual const ShaderDesc& getDesc() const = 0;
	};
}