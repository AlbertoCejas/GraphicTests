#pragma once

#include <cstdint>

namespace potato
{
	struct SwapChainDesc
	{
		int width;
		int height;
	};

    /// Value type
    /// This enumeration describes value type. It is used by
    /// - BufferDesc structure to describe value type of a formatted buffer
    /// - DrawAttribs structure to describe index type for an indexed draw call
    enum ValueType : uint8_t
    {
        VT_UNDEFINED = 0, ///< Undefined type
		VT_INT8,          ///< Signed 8-bit integer
		VT_INT16,         ///< Signed 16-bit integer
		VT_INT32,         ///< Signed 32-bit integer
		VT_UINT8,         ///< Unsigned 8-bit integer
		VT_UINT16,        ///< Unsigned 16-bit integer
		VT_UINT32,        ///< Unsigned 32-bit integer
		VT_FLOAT16,       ///< Half-precision 16-bit floating point
		VT_FLOAT32,       ///< Full-precision 32-bit floating point
		VT_NUM_TYPES      ///< Helper value storing total number of types in the enumeration
    };

	enum TextureFormat : uint16_t
	{
        /// Unknown format
        TEX_FORMAT_UNKNOWN = 0,

        /// Four-component 128-bit typeless format with 32-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R32G32B32A32_TYPELESS. OpenGL does not have direct counterpart, GL_RGBA32F is used.
        TEX_FORMAT_RGBA32_TYPELESS,

        /// Four-component 128-bit floating-point format with 32-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R32G32B32A32_FLOAT. OpenGL counterpart: GL_RGBA32F.
        TEX_FORMAT_RGBA32_FLOAT,

        /// Four-component 128-bit unsigned-integer format with 32-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R32G32B32A32_UINT. OpenGL counterpart: GL_RGBA32UI.
        TEX_FORMAT_RGBA32_UINT,

        /// Four-component 128-bit signed-integer format with 32-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R32G32B32A32_SINT. OpenGL counterpart: GL_RGBA32I.
        TEX_FORMAT_RGBA32_SINT,

        /// Three-component 96-bit typeless format with 32-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R32G32B32_TYPELESS. OpenGL does not have direct counterpart, GL_RGB32F is used.
        /// \warning This format has weak hardware support and is not recommended 
        TEX_FORMAT_RGB32_TYPELESS,

        /// Three-component 96-bit floating-point format with 32-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R32G32B32_FLOAT. OpenGL counterpart: GL_RGB32F.
        /// \warning This format has weak hardware support and is not recommended 
        TEX_FORMAT_RGB32_FLOAT,

        /// Three-component 96-bit unsigned-integer format with 32-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R32G32B32_UINT. OpenGL counterpart: GL_RGB32UI.
        /// \warning This format has weak hardware support and is not recommended 
        TEX_FORMAT_RGB32_UINT,

        /// Three-component 96-bit signed-integer format with 32-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R32G32B32_SINT. OpenGL counterpart: GL_RGB32I.
        /// \warning This format has weak hardware support and is not recommended 
        TEX_FORMAT_RGB32_SINT,

        /// Four-component 64-bit typeless format with 16-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R16G16B16A16_TYPELESS. OpenGL does not have direct counterpart, GL_RGBA16F is used.
        TEX_FORMAT_RGBA16_TYPELESS,

        /// Four-component 64-bit half-precision floating-point format with 16-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R16G16B16A16_FLOAT. OpenGL counterpart: GL_RGBA16F.
        TEX_FORMAT_RGBA16_FLOAT,

        /// Four-component 64-bit unsigned-normalized-integer format with 16-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R16G16B16A16_UNORM. OpenGL counterpart: GL_RGBA16. \n
        /// [GL_EXT_texture_norm16]: https://www.khronos.org/registry/gles/extensions/EXT/EXT_texture_norm16.txt
        /// OpenGLES: [GL_EXT_texture_norm16][] extension is required
        TEX_FORMAT_RGBA16_UNORM,

        /// Four-component 64-bit unsigned-integer format with 16-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R16G16B16A16_UINT. OpenGL counterpart: GL_RGBA16UI.
        TEX_FORMAT_RGBA16_UINT,

        /// [GL_EXT_texture_norm16]: https://www.khronos.org/registry/gles/extensions/EXT/EXT_texture_norm16.txt
        /// Four-component 64-bit signed-normalized-integer format with 16-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R16G16B16A16_SNORM. OpenGL counterpart: GL_RGBA16_SNORM. \n
        /// [GL_EXT_texture_norm16]: https://www.khronos.org/registry/gles/extensions/EXT/EXT_texture_norm16.txt
        /// OpenGLES: [GL_EXT_texture_norm16][] extension is required
        TEX_FORMAT_RGBA16_SNORM,

        /// Four-component 64-bit signed-integer format with 16-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R16G16B16A16_SINT. OpenGL counterpart: GL_RGBA16I.
        TEX_FORMAT_RGBA16_SINT,

        /// Two-component 64-bit typeless format with 32-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R32G32_TYPELESS. OpenGL does not have direct counterpart, GL_RG32F is used.
        TEX_FORMAT_RG32_TYPELESS,

        /// Two-component 64-bit floating-point format with 32-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R32G32_FLOAT. OpenGL counterpart: GL_RG32F.
        TEX_FORMAT_RG32_FLOAT,

        /// Two-component 64-bit unsigned-integer format with 32-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R32G32_UINT. OpenGL counterpart: GL_RG32UI.
        TEX_FORMAT_RG32_UINT,

        /// Two-component 64-bit signed-integer format with 32-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R32G32_SINT. OpenGL counterpart: GL_RG32I.
        TEX_FORMAT_RG32_SINT,

        /// Two-component 64-bit typeless format with 32-bits for R channel and 8 bits for G channel. \n
        /// D3D counterpart: DXGI_FORMAT_R32G8X24_TYPELESS. OpenGL does not have direct counterpart, GL_DEPTH32F_STENCIL8 is used.
        TEX_FORMAT_R32G8X24_TYPELESS,

        /// Two-component 64-bit format with 32-bit floating-point depth channel and 8-bit stencil channel. \n
        /// D3D counterpart: DXGI_FORMAT_D32_FLOAT_S8X24_UINT. OpenGL counterpart: GL_DEPTH32F_STENCIL8.
        TEX_FORMAT_D32_FLOAT_S8X24_UINT,

        /// Two-component 64-bit format with 32-bit floating-point R channel and 8+24-bits of typeless data. \n
        /// D3D counterpart: DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS. OpenGL does not have direct counterpart, GL_DEPTH32F_STENCIL8 is used.
        TEX_FORMAT_R32_FLOAT_X8X24_TYPELESS,

        /// Two-component 64-bit format with 32-bit typeless data and 8-bit G channel. \n
        /// D3D counterpart: DXGI_FORMAT_X32_TYPELESS_G8X24_UINT
        /// \warning This format is currently not implemented in OpenGL version
        TEX_FORMAT_X32_TYPELESS_G8X24_UINT,

        /// Four-component 32-bit typeless format with 10 bits for RGB and 2 bits for alpha channel. \n
        /// D3D counterpart: DXGI_FORMAT_R10G10B10A2_TYPELESS. OpenGL does not have direct counterpart, GL_RGB10_A2 is used.
        TEX_FORMAT_RGB10A2_TYPELESS,

        /// Four-component 32-bit unsigned-normalized-integer format with 10 bits for each color and 2 bits for alpha channel. \n
        /// D3D counterpart: DXGI_FORMAT_R10G10B10A2_UNORM. OpenGL counterpart: GL_RGB10_A2.
        TEX_FORMAT_RGB10A2_UNORM,

        /// Four-component 32-bit unsigned-integer format with 10 bits for each color and 2 bits for alpha channel. \n
        /// D3D counterpart: DXGI_FORMAT_R10G10B10A2_UINT. OpenGL counterpart: GL_RGB10_A2UI.
        TEX_FORMAT_RGB10A2_UINT,

        /// Three-component 32-bit format encoding three partial precision channels using 11 bits for red and green and 10 bits for blue channel. \n
        /// D3D counterpart: DXGI_FORMAT_R11G11B10_FLOAT. OpenGL counterpart: GL_R11F_G11F_B10F.
        TEX_FORMAT_R11G11B10_FLOAT,

        /// Four-component 32-bit typeless format with 8-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R8G8B8A8_TYPELESS. OpenGL does not have direct counterpart, GL_RGBA8 is used.
        TEX_FORMAT_RGBA8_TYPELESS,

        /// Four-component 32-bit unsigned-normalized-integer format with 8-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R8G8B8A8_UNORM. OpenGL counterpart: GL_RGBA8.
        TEX_FORMAT_RGBA8_UNORM,

        /// Four-component 32-bit unsigned-normalized-integer sRGB format with 8-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R8G8B8A8_UNORM_SRGB. OpenGL counterpart: GL_SRGB8_ALPHA8.
        TEX_FORMAT_RGBA8_UNORM_SRGB,

        /// Four-component 32-bit unsigned-integer format with 8-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R8G8B8A8_UINT. OpenGL counterpart: GL_RGBA8UI.
        TEX_FORMAT_RGBA8_UINT,

        /// Four-component 32-bit signed-normalized-integer format with 8-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R8G8B8A8_SNORM. OpenGL counterpart: GL_RGBA8_SNORM.
        TEX_FORMAT_RGBA8_SNORM,

        /// Four-component 32-bit signed-integer format with 8-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R8G8B8A8_SINT. OpenGL counterpart: GL_RGBA8I.
        TEX_FORMAT_RGBA8_SINT,

        /// Two-component 32-bit typeless format with 16-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R16G16_TYPELESS. OpenGL does not have direct counterpart, GL_RG16F is used.
        TEX_FORMAT_RG16_TYPELESS,

        /// Two-component 32-bit half-precision floating-point format with 16-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R16G16_FLOAT. OpenGL counterpart: GL_RG16F.
        TEX_FORMAT_RG16_FLOAT,

        /// Two-component 32-bit unsigned-normalized-integer format with 16-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R16G16_UNORM. OpenGL counterpart: GL_RG16. \n
        /// [GL_EXT_texture_norm16]: https://www.khronos.org/registry/gles/extensions/EXT/EXT_texture_norm16.txt
        /// OpenGLES: [GL_EXT_texture_norm16][] extension is required
        TEX_FORMAT_RG16_UNORM,

        /// Two-component 32-bit unsigned-integer format with 16-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R16G16_UINT. OpenGL counterpart: GL_RG16UI.
        TEX_FORMAT_RG16_UINT,

        /// Two-component 32-bit signed-normalized-integer format with 16-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R16G16_SNORM. OpenGL counterpart: GL_RG16_SNORM. \n
        /// [GL_EXT_texture_norm16]: https://www.khronos.org/registry/gles/extensions/EXT/EXT_texture_norm16.txt
        /// OpenGLES: [GL_EXT_texture_norm16][] extension is required
        TEX_FORMAT_RG16_SNORM,

        /// Two-component 32-bit signed-integer format with 16-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R16G16_SINT. OpenGL counterpart: GL_RG16I.
        TEX_FORMAT_RG16_SINT,

        /// Single-component 32-bit typeless format. \n
        /// D3D counterpart: DXGI_FORMAT_R32_TYPELESS. OpenGL does not have direct counterpart, GL_R32F is used.
        TEX_FORMAT_R32_TYPELESS,

        /// Single-component 32-bit floating-point depth format. \n
        /// D3D counterpart: DXGI_FORMAT_D32_FLOAT. OpenGL counterpart: GL_DEPTH_COMPONENT32F.
        TEX_FORMAT_D32_FLOAT,

        /// Single-component 32-bit floating-point format. \n
        /// D3D counterpart: DXGI_FORMAT_R32_FLOAT. OpenGL counterpart: GL_R32F.
        TEX_FORMAT_R32_FLOAT,

        /// Single-component 32-bit unsigned-integer format. \n
        /// D3D counterpart: DXGI_FORMAT_R32_UINT. OpenGL counterpart: GL_R32UI.
        TEX_FORMAT_R32_UINT,

        /// Single-component 32-bit signed-integer format. \n
        /// D3D counterpart: DXGI_FORMAT_R32_SINT. OpenGL counterpart: GL_R32I.
        TEX_FORMAT_R32_SINT,

        /// Two-component 32-bit typeless format with 24 bits for R and 8 bits for G channel. \n
        /// D3D counterpart: DXGI_FORMAT_R24G8_TYPELESS. OpenGL does not have direct counterpart, GL_DEPTH24_STENCIL8 is used.
        TEX_FORMAT_R24G8_TYPELESS,

        /// Two-component 32-bit format with 24 bits for unsigned-normalized-integer depth and 8 bits for stencil. \n
        /// D3D counterpart: DXGI_FORMAT_D24_UNORM_S8_UINT. OpenGL counterpart: GL_DEPTH24_STENCIL8.
        TEX_FORMAT_D24_UNORM_S8_UINT,

        /// Two-component 32-bit format with 24 bits for unsigned-normalized-integer data and 8 bits of unreferenced data. \n
        /// D3D counterpart: DXGI_FORMAT_R24_UNORM_X8_TYPELESS. OpenGL does not have direct counterpart, GL_DEPTH24_STENCIL8 is used.
        TEX_FORMAT_R24_UNORM_X8_TYPELESS,

        /// Two-component 32-bit format with 24 bits of unreferenced data and 8 bits of unsigned-integer data. \n
        /// D3D counterpart: DXGI_FORMAT_X24_TYPELESS_G8_UINT
        /// \warning This format is currently not implemented in OpenGL version
        TEX_FORMAT_X24_TYPELESS_G8_UINT,

        /// Two-component 16-bit typeless format with 8-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R8G8_TYPELESS. OpenGL does not have direct counterpart, GL_RG8 is used.
        TEX_FORMAT_RG8_TYPELESS,

        /// Two-component 16-bit unsigned-normalized-integer format with 8-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R8G8_UNORM. OpenGL counterpart: GL_RG8.
        TEX_FORMAT_RG8_UNORM,

        /// Two-component 16-bit unsigned-integer format with 8-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R8G8_UINT. OpenGL counterpart: GL_RG8UI.
        TEX_FORMAT_RG8_UINT,

        /// Two-component 16-bit signed-normalized-integer format with 8-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R8G8_SNORM. OpenGL counterpart: GL_RG8_SNORM.
        TEX_FORMAT_RG8_SNORM,

        /// Two-component 16-bit signed-integer format with 8-bit channels. \n
        /// D3D counterpart: DXGI_FORMAT_R8G8_SINT. OpenGL counterpart: GL_RG8I.
        TEX_FORMAT_RG8_SINT,

        /// Single-component 16-bit typeless format. \n
        /// D3D counterpart: DXGI_FORMAT_R16_TYPELESS. OpenGL does not have direct counterpart, GL_R16F is used.
        TEX_FORMAT_R16_TYPELESS,

        /// Single-component 16-bit half-precisoin floating-point format. \n
        /// D3D counterpart: DXGI_FORMAT_R16_FLOAT. OpenGL counterpart: GL_R16F.
        TEX_FORMAT_R16_FLOAT,

        /// Single-component 16-bit unsigned-normalized-integer depth format. \n
        /// D3D counterpart: DXGI_FORMAT_D16_UNORM. OpenGL counterpart: GL_DEPTH_COMPONENT16.
        TEX_FORMAT_D16_UNORM,

        /// Single-component 16-bit unsigned-normalized-integer format. \n
        /// D3D counterpart: DXGI_FORMAT_R16_UNORM. OpenGL counterpart: GL_R16.
        /// [GL_EXT_texture_norm16]: https://www.khronos.org/registry/gles/extensions/EXT/EXT_texture_norm16.txt
        /// OpenGLES: [GL_EXT_texture_norm16][] extension is required
        TEX_FORMAT_R16_UNORM,

        /// Single-component 16-bit unsigned-integer format. \n
        /// D3D counterpart: DXGI_FORMAT_R16_UINT. OpenGL counterpart: GL_R16UI.
        TEX_FORMAT_R16_UINT,

        /// Single-component 16-bit signed-normalized-integer format. \n
        /// D3D counterpart: DXGI_FORMAT_R16_SNORM. OpenGL counterpart: GL_R16_SNORM. \n
        /// [GL_EXT_texture_norm16]: https://www.khronos.org/registry/gles/extensions/EXT/EXT_texture_norm16.txt
        /// OpenGLES: [GL_EXT_texture_norm16][] extension is required
        TEX_FORMAT_R16_SNORM,

        /// Single-component 16-bit signed-integer format. \n
        /// D3D counterpart: DXGI_FORMAT_R16_SINT. OpenGL counterpart: GL_R16I.
        TEX_FORMAT_R16_SINT,

        /// Single-component 8-bit typeless format. \n
        /// D3D counterpart: DXGI_FORMAT_R8_TYPELESS. OpenGL does not have direct counterpart, GL_R8 is used.
        TEX_FORMAT_R8_TYPELESS,

        /// Single-component 8-bit unsigned-normalized-integer format. \n
        /// D3D counterpart: DXGI_FORMAT_R8_UNORM. OpenGL counterpart: GL_R8.
        TEX_FORMAT_R8_UNORM,

        /// Single-component 8-bit unsigned-integer format. \n
        /// D3D counterpart: DXGI_FORMAT_R8_UINT. OpenGL counterpart: GL_R8UI.
        TEX_FORMAT_R8_UINT,

        /// Single-component 8-bit signed-normalized-integer format. \n
        /// D3D counterpart: DXGI_FORMAT_R8_SNORM. OpenGL counterpart: GL_R8_SNORM.
        TEX_FORMAT_R8_SNORM,

        /// Single-component 8-bit signed-integer format. \n
        /// D3D counterpart: DXGI_FORMAT_R8_SINT. OpenGL counterpart: GL_R8I.
        TEX_FORMAT_R8_SINT,

        /// Single-component 8-bit unsigned-normalized-integer format for alpha only. \n
        /// D3D counterpart: DXGI_FORMAT_A8_UNORM
        /// \warning This format is not availanle in OpenGL
        TEX_FORMAT_A8_UNORM,

        /// Single-component 1-bit format. \n
        /// D3D counterpart: DXGI_FORMAT_R1_UNORM
        /// \warning This format is not availanle in OpenGL
        TEX_FORMAT_R1_UNORM,

        /// Three partial-precision floating pointer numbers sharing single exponent encoded into a 32-bit value. \n
        /// D3D counterpart: DXGI_FORMAT_R9G9B9E5_SHAREDEXP. OpenGL counterpart: GL_RGB9_E5.
        TEX_FORMAT_RGB9E5_SHAREDEXP,

        /// Four-component unsigned-normalized integer format analogous to UYVY encoding. \n
        /// D3D counterpart: DXGI_FORMAT_R8G8_B8G8_UNORM
        /// \warning This format is not availanle in OpenGL
        TEX_FORMAT_RG8_B8G8_UNORM,

        /// Four-component unsigned-normalized integer format analogous to YUY2 encoding. \n
        /// D3D counterpart: DXGI_FORMAT_G8R8_G8B8_UNORM
        /// \warning This format is not availanle in OpenGL
        TEX_FORMAT_G8R8_G8B8_UNORM,

        /// Four-component typeless block-compression format with 1:8 compression ratio.\n
        /// D3D counterpart: DXGI_FORMAT_BC1_TYPELESS. OpenGL does not have direct counterpart, GL_COMPRESSED_RGB_S3TC_DXT1_EXT is used. \n
        /// [GL_EXT_texture_compression_s3tc]: https://www.khronos.org/registry/gles/extensions/EXT/texture_compression_s3tc.txt
        /// OpenGL & OpenGLES: [GL_EXT_texture_compression_s3tc][] extension is required
        /// \sa <a href = "https://docs.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-block-compression#bc1">BC1 on MSDN </a>, 
        ///     <a href = "https://www.opengl.org/wiki/S3_Texture_Compression#DXT1_Format">DXT1 on OpenGL.org </a>
        TEX_FORMAT_BC1_TYPELESS,

        /// Four-component unsigned-normalized-integer block-compression format with 5 bits for R, 6 bits for G, 5 bits for B, and 0 or 1 bit for A channel. 
        /// The pixel data is encoded using 8 bytes per 4x4 block (4 bits per pixel) providing 1:8 compression ratio against RGBA8 format. \n
        /// D3D counterpart: DXGI_FORMAT_BC1_UNORM. OpenGL counterpart: GL_COMPRESSED_RGB_S3TC_DXT1_EXT.\n
        /// [GL_EXT_texture_compression_s3tc]: https://www.khronos.org/registry/gles/extensions/EXT/texture_compression_s3tc.txt
        /// OpenGL & OpenGLES: [GL_EXT_texture_compression_s3tc][] extension is required
        /// \sa <a href = "https://docs.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-block-compression#bc1">BC1 on MSDN </a>, 
        ///     <a href = "https://www.opengl.org/wiki/S3_Texture_Compression#DXT1_Format">DXT1 on OpenGL.org </a>
        TEX_FORMAT_BC1_UNORM,

        /// Four-component unsigned-normalized-integer block-compression sRGB format with 5 bits for R, 6 bits for G, 5 bits for B, and 0 or 1 bit for A channel. \n
        /// The pixel data is encoded using 8 bytes per 4x4 block (4 bits per pixel) providing 1:8 compression ratio against RGBA8 format. \n
        /// D3D counterpart: DXGI_FORMAT_BC1_UNORM_SRGB. OpenGL counterpart: GL_COMPRESSED_SRGB_S3TC_DXT1_EXT.\n
        /// [GL_EXT_texture_compression_s3tc]: https://www.khronos.org/registry/gles/extensions/EXT/texture_compression_s3tc.txt
        /// OpenGL & OpenGLES: [GL_EXT_texture_compression_s3tc][] extension is required
        /// \sa <a href = "https://docs.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-block-compression#bc1">BC1 on MSDN </a>, 
        ///     <a href = "https://www.opengl.org/wiki/S3_Texture_Compression#DXT1_Format">DXT1 on OpenGL.org </a>
        TEX_FORMAT_BC1_UNORM_SRGB,

        /// Four component typeless block-compression format with 1:4 compression ratio.\n
        /// D3D counterpart: DXGI_FORMAT_BC2_TYPELESS. OpenGL does not have direct counterpart, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT is used. \n 
        /// [GL_EXT_texture_compression_s3tc]: https://www.khronos.org/registry/gles/extensions/EXT/texture_compression_s3tc.txt
        /// OpenGL & OpenGLES: [GL_EXT_texture_compression_s3tc][] extension is required
        /// \sa <a href = "https://docs.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-block-compression#bc2">BC2 on MSDN </a>, 
        ///     <a href = "https://www.opengl.org/wiki/S3_Texture_Compression#DXT3_Format">DXT3 on OpenGL.org </a>
        TEX_FORMAT_BC2_TYPELESS,

        /// Four-component unsigned-normalized-integer block-compression format with 5 bits for R, 6 bits for G, 5 bits for B, and 4 bits for low-coherent separate A channel.
        /// The pixel data is encoded using 16 bytes per 4x4 block (8 bits per pixel) providing 1:4 compression ratio against RGBA8 format. \n
        /// D3D counterpart: DXGI_FORMAT_BC2_UNORM. OpenGL counterpart: GL_COMPRESSED_RGBA_S3TC_DXT3_EXT. \n
        /// [GL_EXT_texture_compression_s3tc]: https://www.khronos.org/registry/gles/extensions/EXT/texture_compression_s3tc.txt
        /// OpenGL & OpenGLES: [GL_EXT_texture_compression_s3tc][] extension is required
        /// \sa <a href = "https://docs.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-block-compression#bc2">BC2 on MSDN </a>, 
        ///     <a href = "https://www.opengl.org/wiki/S3_Texture_Compression#DXT3_Format">DXT3 on OpenGL.org </a>
        TEX_FORMAT_BC2_UNORM,

        /// Four-component signed-normalized-integer block-compression sRGB format with 5 bits for R, 6 bits for G, 5 bits for B, and 4 bits for low-coherent separate A channel.
        /// The pixel data is encoded using 16 bytes per 4x4 block (8 bits per pixel) providing 1:4 compression ratio against RGBA8 format. \n
        /// D3D counterpart: DXGI_FORMAT_BC2_UNORM_SRGB. OpenGL counterpart: GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT. \n
        /// [GL_EXT_texture_compression_s3tc]: https://www.khronos.org/registry/gles/extensions/EXT/texture_compression_s3tc.txt
        /// OpenGL & OpenGLES: [GL_EXT_texture_compression_s3tc][] extension is required
        /// \sa <a href = "https://docs.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-block-compression#bc2">BC2 on MSDN </a>, 
        ///     <a href = "https://www.opengl.org/wiki/S3_Texture_Compression#DXT3_Format">DXT3 on OpenGL.org </a>
        TEX_FORMAT_BC2_UNORM_SRGB,

        /// Four-component typeless block-compression format with 1:4 compression ratio.\n
        /// D3D counterpart: DXGI_FORMAT_BC3_TYPELESS. OpenGL does not have direct counterpart, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT is used. \n
        /// [GL_EXT_texture_compression_s3tc]: https://www.khronos.org/registry/gles/extensions/EXT/texture_compression_s3tc.txt
        /// OpenGL & OpenGLES: [GL_EXT_texture_compression_s3tc][] extension is required
        /// \sa <a href = "https://docs.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-block-compression#bc3">BC3 on MSDN </a>, 
        ///     <a href = "https://www.opengl.org/wiki/S3_Texture_Compression#DXT5_Format">DXT5 on OpenGL.org </a>
        TEX_FORMAT_BC3_TYPELESS,

        /// Four-component unsigned-normalized-integer block-compression format with 5 bits for R, 6 bits for G, 5 bits for B, and 8 bits for highly-coherent A channel.
        /// The pixel data is encoded using 16 bytes per 4x4 block (8 bits per pixel) providing 1:4 compression ratio against RGBA8 format. \n
        /// D3D counterpart: DXGI_FORMAT_BC3_UNORM. OpenGL counterpart: GL_COMPRESSED_RGBA_S3TC_DXT5_EXT. \n
        /// [GL_EXT_texture_compression_s3tc]: https://www.khronos.org/registry/gles/extensions/EXT/texture_compression_s3tc.txt
        /// OpenGL & OpenGLES: [GL_EXT_texture_compression_s3tc][] extension is required
        /// \sa <a href = "https://docs.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-block-compression#bc3">BC3 on MSDN </a>, 
        ///     <a href = "https://www.opengl.org/wiki/S3_Texture_Compression#DXT5_Format">DXT5 on OpenGL.org </a>
        TEX_FORMAT_BC3_UNORM,

        /// Four-component unsigned-normalized-integer block-compression sRGB format with 5 bits for R, 6 bits for G, 5 bits for B, and 8 bits for highly-coherent A channel.
        /// The pixel data is encoded using 16 bytes per 4x4 block (8 bits per pixel) providing 1:4 compression ratio against RGBA8 format. \n
        /// D3D counterpart: DXGI_FORMAT_BC3_UNORM_SRGB. OpenGL counterpart: GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT. \n
        /// [GL_EXT_texture_compression_s3tc]: https://www.khronos.org/registry/gles/extensions/EXT/texture_compression_s3tc.txt
        /// OpenGL & OpenGLES: [GL_EXT_texture_compression_s3tc][] extension is required
        /// \sa <a href = "https://docs.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-block-compression#bc3">BC3 on MSDN </a>, 
        ///     <a href = "https://www.opengl.org/wiki/S3_Texture_Compression#DXT5_Format">DXT5 on OpenGL.org </a>
        TEX_FORMAT_BC3_UNORM_SRGB,

        /// One-component typeless block-compression format with 1:2 compression ratio. \n
        /// D3D counterpart: DXGI_FORMAT_BC4_TYPELESS. OpenGL does not have direct counterpart, GL_COMPRESSED_RED_RGTC1 is used. \n
        /// [GL_ARB_texture_compression_rgtc]: https://www.opengl.org/registry/specs/ARB/texture_compression_rgtc.txt
        /// OpenGL & OpenGLES: [GL_ARB_texture_compression_rgtc][] extension is required
        /// \sa <a href = "https://docs.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-block-compression#bc4">BC4 on MSDN </a>, 
        ///     <a href = "https://www.opengl.org/wiki/Image_Format#Compressed_formats">Compressed formats on OpenGL.org </a>
        TEX_FORMAT_BC4_TYPELESS,

        /// One-component unsigned-normalized-integer block-compression format with 8 bits for R channel.
        /// The pixel data is encoded using 8 bytes per 4x4 block (4 bits per pixel) providing 1:2 compression ratio against R8 format. \n
        /// D3D counterpart: DXGI_FORMAT_BC4_UNORM. OpenGL counterpart: GL_COMPRESSED_RED_RGTC1. \n
        /// [GL_ARB_texture_compression_rgtc]: https://www.opengl.org/registry/specs/ARB/texture_compression_rgtc.txt
        /// OpenGL & OpenGLES: [GL_ARB_texture_compression_rgtc][] extension is required
        /// \sa <a href = "https://docs.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-block-compression#bc4">BC4 on MSDN </a>, 
        ///     <a href = "https://www.opengl.org/wiki/Image_Format#Compressed_formats">Compressed formats on OpenGL.org </a>
        TEX_FORMAT_BC4_UNORM,

        /// One-component signed-normalized-integer block-compression format with 8 bits for R channel.
        /// The pixel data is encoded using 8 bytes per 4x4 block (4 bits per pixel) providing 1:2 compression ratio against R8 format. \n
        /// D3D counterpart: DXGI_FORMAT_BC4_SNORM. OpenGL counterpart: GL_COMPRESSED_SIGNED_RED_RGTC1. \n
        /// [GL_ARB_texture_compression_rgtc]: https://www.opengl.org/registry/specs/ARB/texture_compression_rgtc.txt
        /// OpenGL & OpenGLES: [GL_ARB_texture_compression_rgtc][] extension is required
        /// \sa <a href = "https://docs.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-block-compression#bc4">BC4 on MSDN </a>, 
        ///     <a href = "https://www.opengl.org/wiki/Image_Format#Compressed_formats">Compressed formats on OpenGL.org </a>
        TEX_FORMAT_BC4_SNORM,

        /// Two-component typeless block-compression format with 1:2 compression ratio. \n
        /// D3D counterpart: DXGI_FORMAT_BC5_TYPELESS. OpenGL does not have direct counterpart, GL_COMPRESSED_RG_RGTC2 is used. \n
        /// [GL_ARB_texture_compression_rgtc]: https://www.opengl.org/registry/specs/ARB/texture_compression_rgtc.txt
        /// OpenGL & OpenGLES: [GL_ARB_texture_compression_rgtc][] extension is required
        /// \sa <a href = "https://docs.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-block-compression#bc5">BC5 on MSDN </a>, 
        ///     <a href = "https://www.opengl.org/wiki/Image_Format#Compressed_formats">Compressed formats on OpenGL.org </a>
        TEX_FORMAT_BC5_TYPELESS,

        /// Two-component unsigned-normalized-integer block-compression format with 8 bits for R and 8 bits for G channel.
        /// The pixel data is encoded using 16 bytes per 4x4 block (8 bits per pixel) providing 1:2 compression ratio against RG8 format. \n
        /// D3D counterpart: DXGI_FORMAT_BC5_UNORM. OpenGL counterpart: GL_COMPRESSED_RG_RGTC2. \n
        /// [GL_ARB_texture_compression_rgtc]: https://www.opengl.org/registry/specs/ARB/texture_compression_rgtc.txt
        /// OpenGL & OpenGLES: [GL_ARB_texture_compression_rgtc][] extension is required
        /// \sa <a href = "https://docs.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-block-compression#bc5">BC5 on MSDN </a>, 
        ///     <a href = "https://www.opengl.org/wiki/Image_Format#Compressed_formats">Compressed formats on OpenGL.org </a>
        TEX_FORMAT_BC5_UNORM,

        /// Two-component signed-normalized-integer block-compression format with 8 bits for R and 8 bits for G channel.
        /// The pixel data is encoded using 16 bytes per 4x4 block (8 bits per pixel) providing 1:2 compression ratio against RG8 format. \n
        /// D3D counterpart: DXGI_FORMAT_BC5_SNORM. OpenGL counterpart: GL_COMPRESSED_SIGNED_RG_RGTC2. \n
        /// [GL_ARB_texture_compression_rgtc]: https://www.opengl.org/registry/specs/ARB/texture_compression_rgtc.txt
        /// OpenGL & OpenGLES: [GL_ARB_texture_compression_rgtc][] extension is required
        /// \sa <a href = "https://docs.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-block-compression#bc5">BC5 on MSDN </a>, 
        ///     <a href = "https://www.opengl.org/wiki/Image_Format#Compressed_formats">Compressed formats on OpenGL.org </a>
        TEX_FORMAT_BC5_SNORM,

        /// Three-component 16-bit unsigned-normalized-integer format with 5 bits for blue, 6 bits for green, and 5 bits for red channel. \n
        /// D3D counterpart: DXGI_FORMAT_B5G6R5_UNORM
        /// \warning This format is not available until D3D11.1 and Windows 8. It is also not available in OpenGL
        TEX_FORMAT_B5G6R5_UNORM,

        /// Four-component 16-bit unsigned-normalized-integer format with 5 bits for each color channel and 1-bit alpha. \n
        /// D3D counterpart: DXGI_FORMAT_B5G5R5A1_UNORM
        /// \warning This format is not available until D3D11.1 and Windows 8. It is also not available in OpenGL
        TEX_FORMAT_B5G5R5A1_UNORM,

        /// Four-component 32-bit unsigned-normalized-integer format with 8 bits for each channel. \n
        /// D3D counterpart: DXGI_FORMAT_B8G8R8A8_UNORM.
        /// \warning This format is not available in OpenGL
        TEX_FORMAT_BGRA8_UNORM,

        /// Four-component 32-bit unsigned-normalized-integer format with 8 bits for each color channel and 8 bits unused. \n
        /// D3D counterpart: DXGI_FORMAT_B8G8R8X8_UNORM.
        /// \warning This format is not available in OpenGL
        TEX_FORMAT_BGRX8_UNORM,

        /// Four-component 32-bit 2.8-biased fixed-point format with 10 bits for each color channel and 2-bit alpha. \n
        /// D3D counterpart: DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM.
        /// \warning This format is not available in OpenGL
        TEX_FORMAT_R10G10B10_XR_BIAS_A2_UNORM,

        /// Four-component 32-bit typeless format with 8 bits for each channel. \n
        /// D3D counterpart: DXGI_FORMAT_B8G8R8A8_TYPELESS.
        /// \warning This format is not available in OpenGL
        TEX_FORMAT_BGRA8_TYPELESS,

        /// Four-component 32-bit unsigned-normalized sRGB format with 8 bits for each channel. \n
        /// D3D counterpart: DXGI_FORMAT_B8G8R8A8_UNORM_SRGB.
        /// \warning This format is not available in OpenGL.
        TEX_FORMAT_BGRA8_UNORM_SRGB,

        /// Four-component 32-bit typeless format that with 8 bits for each color channel, and 8 bits are unused. \n
        /// D3D counterpart: DXGI_FORMAT_B8G8R8X8_TYPELESS.
        /// \warning This format is not available in OpenGL.
        TEX_FORMAT_BGRX8_TYPELESS,

        /// Four-component 32-bit unsigned-normalized sRGB format with 8 bits for each color channel, and 8 bits are unused. \n
        /// D3D counterpart: DXGI_FORMAT_B8G8R8X8_UNORM_SRGB.
        /// \warning This format is not available in OpenGL.
        TEX_FORMAT_BGRX8_UNORM_SRGB,

        /// Three-component typeless block-compression format. \n
        /// D3D counterpart: DXGI_FORMAT_BC6H_TYPELESS. OpenGL does not have direct counterpart, GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT is used. \n
        /// [GL_ARB_texture_compression_bptc]: https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_texture_compression_bptc.txt
        /// OpenGL: [GL_ARB_texture_compression_bptc][] extension is required. Not supported in at least OpenGLES3.1
        /// \sa <a href = "https://docs.microsoft.com/en-us/windows/win32/direct3d11/bc6h-format">BC6H on MSDN </a>, 
        ///     <a href = "https://www.opengl.org/wiki/BPTC_Texture_Compression">BPTC Texture Compression on OpenGL.org </a>
        TEX_FORMAT_BC6H_TYPELESS,

        /// Three-component unsigned half-precision floating-point format with 16 bits for each channel. \n
        /// D3D counterpart: DXGI_FORMAT_BC6H_UF16. OpenGL counterpart: GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT. \n
        /// [GL_ARB_texture_compression_bptc]: https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_texture_compression_bptc.txt
        /// OpenGL: [GL_ARB_texture_compression_bptc][] extension is required. Not supported in at least OpenGLES3.1
        /// \sa <a href = "https://docs.microsoft.com/en-us/windows/win32/direct3d11/bc6h-format">BC6H on MSDN </a>, 
        ///     <a href = "https://www.opengl.org/wiki/BPTC_Texture_Compression">BPTC Texture Compression on OpenGL.org </a>
        TEX_FORMAT_BC6H_UF16,

        /// Three-channel signed half-precision floating-point format with 16 bits per each channel. \n
        /// D3D counterpart: DXGI_FORMAT_BC6H_SF16. OpenGL counterpart: GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT. \n
        /// [GL_ARB_texture_compression_bptc]: https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_texture_compression_bptc.txt
        /// OpenGL: [GL_ARB_texture_compression_bptc][] extension is required. Not supported in at least OpenGLES3.1
        /// \sa <a href = "https://docs.microsoft.com/en-us/windows/win32/direct3d11/bc6h-format">BC6H on MSDN </a>, 
        ///     <a href = "https://www.opengl.org/wiki/BPTC_Texture_Compression">BPTC Texture Compression on OpenGL.org </a>
        TEX_FORMAT_BC6H_SF16,

        /// Three-component typeless block-compression format. \n
        /// D3D counterpart: DXGI_FORMAT_BC7_TYPELESS. OpenGL does not have direct counterpart, GL_COMPRESSED_RGBA_BPTC_UNORM is used. \n
        /// [GL_ARB_texture_compression_bptc]: https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_texture_compression_bptc.txt
        /// OpenGL: [GL_ARB_texture_compression_bptc][] extension is required. Not supported in at least OpenGLES3.1
        /// \sa <a href = "https://docs.microsoft.com/en-us/windows/win32/direct3d11/bc7-format">BC7 on MSDN </a>, 
        ///     <a href = "https://www.opengl.org/wiki/BPTC_Texture_Compression">BPTC Texture Compression on OpenGL.org </a>
        TEX_FORMAT_BC7_TYPELESS,

        /// Three-component block-compression unsigned-normalized-integer format with 4 to 7 bits per color channel and 0 to 8 bits of alpha. \n
        /// D3D counterpart: DXGI_FORMAT_BC7_UNORM. OpenGL counterpart: GL_COMPRESSED_RGBA_BPTC_UNORM. \n
        /// [GL_ARB_texture_compression_bptc]: https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_texture_compression_bptc.txt
        /// OpenGL: [GL_ARB_texture_compression_bptc][] extension is required. Not supported in at least OpenGLES3.1
        /// \sa <a href = "https://docs.microsoft.com/en-us/windows/win32/direct3d11/bc7-format">BC7 on MSDN </a>, 
        ///     <a href = "https://www.opengl.org/wiki/BPTC_Texture_Compression">BPTC Texture Compression on OpenGL.org </a>
        TEX_FORMAT_BC7_UNORM,

        /// Three-component block-compression unsigned-normalized-integer sRGB format with 4 to 7 bits per color channel and 0 to 8 bits of alpha. \n
        /// D3D counterpart: DXGI_FORMAT_BC7_UNORM_SRGB. OpenGL counterpart: GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM. \n
        /// [GL_ARB_texture_compression_bptc]: https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_texture_compression_bptc.txt
        /// OpenGL: [GL_ARB_texture_compression_bptc][] extension is required. Not supported in at least OpenGLES3.1
        /// \sa <a href = "https://docs.microsoft.com/en-us/windows/win32/direct3d11/bc7-format">BC7 on MSDN </a>, 
        ///     <a href = "https://www.opengl.org/wiki/BPTC_Texture_Compression">BPTC Texture Compression on OpenGL.org </a>
        TEX_FORMAT_BC7_UNORM_SRGB,

        /// Helper member containing the total number of texture formats in the enumeration
        TEX_FORMAT_NUM_FORMATS
	};

    /// Resource usage stage
    enum ResourceState : uint32_t
    {
        /// The resource state is not known to the engine and is managed by the application
        RESOURCE_STATE_UNKNOWN = 0x00000,

        /// The resource state is known to the engine, but is undefined. A resource is typically in an undefined state right after initialization.
        RESOURCE_STATE_UNDEFINED = 0x00001,

        /// The resource is accessed as a vertex buffer
        RESOURCE_STATE_VERTEX_BUFFER = 0x00002,

        /// The resource is accessed as a constant (uniform) buffer
        RESOURCE_STATE_CONSTANT_BUFFER = 0x00004,

        /// The resource is accessed as an index buffer
        RESOURCE_STATE_INDEX_BUFFER = 0x00008,

        /// The resource is accessed as a render target
        RESOURCE_STATE_RENDER_TARGET = 0x00010,

        /// The resource is used for unordered access
        RESOURCE_STATE_UNORDERED_ACCESS = 0x00020,

        /// The resource is used in a writable depth-stencil view or in clear operation
        RESOURCE_STATE_DEPTH_WRITE = 0x00040,

        /// The resource is used in a read-only depth-stencil view
        RESOURCE_STATE_DEPTH_READ = 0x00080,

        /// The resource is accessed from a shader
        RESOURCE_STATE_SHADER_RESOURCE = 0x00100,

        /// The resource is used as the destination for stream output
        RESOURCE_STATE_STREAM_OUT = 0x00200,

        /// The resource is used as an indirect draw/dispatch arguments buffer
        RESOURCE_STATE_INDIRECT_ARGUMENT = 0x00400,

        /// The resource is used as the destination in a copy operation
        RESOURCE_STATE_COPY_DEST = 0x00800,

        /// The resource is used as the source in a copy operation 
        RESOURCE_STATE_COPY_SOURCE = 0x01000,

        /// The resource is used as the destination in a resolve operation 
        RESOURCE_STATE_RESOLVE_DEST = 0x02000,

        /// The resource is used as the source in a resolve operation 
        RESOURCE_STATE_RESOLVE_SOURCE = 0x04000,

        /// The resource is used as an input attachment in a render pass subpass
        RESOURCE_STATE_INPUT_ATTACHMENT = 0x08000,

        /// The resource is used for present
        RESOURCE_STATE_PRESENT = 0x10000,

        /// The resource is used as vertex/index/instance buffer in an AS building operation
        /// or as an acceleration structure source in an AS copy operation.
        RESOURCE_STATE_BUILD_AS_READ = 0x20000,

        /// The resource is used as the target for AS building or AS copy operations.
        RESOURCE_STATE_BUILD_AS_WRITE = 0x40000,

        /// The resource is used as a top-level AS shader resource in a trace rays operation.
        RESOURCE_STATE_RAY_TRACING = 0x80000,

        RESOURCE_STATE_MAX_BIT = RESOURCE_STATE_RAY_TRACING,

        RESOURCE_STATE_GENERIC_READ = RESOURCE_STATE_VERTEX_BUFFER |
        RESOURCE_STATE_CONSTANT_BUFFER |
        RESOURCE_STATE_INDEX_BUFFER |
        RESOURCE_STATE_SHADER_RESOURCE |
        RESOURCE_STATE_INDIRECT_ARGUMENT |
        RESOURCE_STATE_COPY_SOURCE
    };

    /// Pipeline stage flags.

    /// These flags mirror [VkPipelineStageFlagBits](https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VkPipelineStageFlagBits)
    /// enum and only have effect in Vulkan backend.
    enum PipelineStageFlags : uint32_t
    {
        /// Undefined stage
        PIPELINE_STAGE_FLAG_UNDEFINED = 0x00000000,

		/// The top of the pipeline.
		PIPELINE_STAGE_FLAG_TOP_OF_PIPE = 0x00000001,

		/// The stage of the pipeline where Draw/DispatchIndirect data structures are consumed.
		PIPELINE_STAGE_FLAG_DRAW_INDIRECT = 0x00000002,

		/// The stage of the pipeline where vertex and index buffers are consumed.
		PIPELINE_STAGE_FLAG_VERTEX_INPUT = 0x00000004,

		/// Vertex shader stage.
		PIPELINE_STAGE_FLAG_VERTEX_SHADER = 0x00000008,

		/// Hull shader stage.
		PIPELINE_STAGE_FLAG_HULL_SHADER = 0x00000010,

		/// Domain shader stage.
		PIPELINE_STAGE_FLAG_DOMAIN_SHADER = 0x00000020,

		/// Geometry shader stage.
		PIPELINE_STAGE_FLAG_GEOMETRY_SHADER = 0x00000040,

		/// Pixel shader stage.
		PIPELINE_STAGE_FLAG_PIXEL_SHADER = 0x00000080,

		/// The stage of the pipeline where early fragment tests (depth and
		/// stencil tests before fragment shading) are performed. This stage
		/// also includes subpass load operations for framebuffer attachments
		/// with a depth/stencil format.
		PIPELINE_STAGE_FLAG_EARLY_FRAGMENT_TESTS = 0x00000100,

		/// The stage of the pipeline where late fragment tests (depth and 
		/// stencil tests after fragment shading) are performed. This stage
		/// also includes subpass store operations for framebuffer attachments
		/// with a depth/stencil format.
		PIPELINE_STAGE_FLAG_LATE_FRAGMENT_TESTS = 0x00000200,

		/// The stage of the pipeline after blending where the final color values
		/// are output from the pipeline. This stage also includes subpass load
		/// and store operations and multisample resolve operations for framebuffer
		/// attachments with a color or depth/stencil format.
		PIPELINE_STAGE_FLAG_RENDER_TARGET = 0x00000400,

		/// Compute shader stage.
		PIPELINE_STAGE_FLAG_COMPUTE_SHADER = 0x00000800,

		/// The stage where all copy and outside-of-renderpass
		/// resolve and clear operations happen.
		PIPELINE_STAGE_FLAG_TRANSFER = 0x00001000,

		/// The bottom of the pipeline.
		PIPELINE_STAGE_FLAG_BOTTOM_OF_PIPE = 0x00002000,

		/// A pseudo-stage indicating execution on the host of reads/writes
		/// of device memory. This stage is not invoked by any commands recorded
		/// in a command buffer.
		PIPELINE_STAGE_FLAG_HOST = 0x00004000,

		/// The stage of the pipeline where the predicate of conditional rendering is consumed.
		PIPELINE_STAGE_FLAG_CONDITIONAL_RENDERING = 0x00040000,

		/// The stage of the pipeline where the shading rate texture is
		/// read to determine the shading rate for portions of a rasterized primitive.
		PIPELINE_STAGE_FLAG_SHADING_RATE_TEXTURE = 0x00400000,

		/// Ray tracing shader.
		PIPELINE_STAGE_FLAG_RAY_TRACING_SHADER = 0x00200000,

		/// Acceleration structure build shader.
		PIPELINE_STAGE_FLAG_ACCELERATION_STRUCTURE_BUILD = 0x02000000,

		/// Task shader stage.
		PIPELINE_STAGE_FLAG_TASK_SHADER = 0x00080000,

		/// Mesh shader stage.
		PIPELINE_STAGE_FLAG_MESH_SHADER = 0x00100000,

		/// 
		PIPELINE_STAGE_FLAG_FRAGMENT_DENSITY_PROCESS = 0x00800000,

		/// Default pipeline stage that is determined by the resource state.
		/// For example, RESOURCE_STATE_RENDER_TARGET corresponds to
		/// PIPELINE_STAGE_FLAG_RENDER_TARGET pipeline stage.
		PIPELINE_STAGE_FLAG_DEFAULT = 0x80000000
    };

    /// Access flag.

    /// The flags mirror [VkAccessFlags](https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VkAccessFlags) enum
    /// and only have effect in Vulkan backend.
    enum AccessFlags : uint32_t
    {
		/// No access
		ACCESS_FLAG_NONE = 0x00000000,

		/// Read access to indirect command data read as part of an indirect
		/// drawing or dispatch command.
		ACCESS_FLAG_INDIRECT_COMMAND_READ = 0x00000001,

		/// Read access to an index buffer as part of an indexed drawing command
		ACCESS_FLAG_INDEX_READ = 0x00000002,

		/// Read access to a vertex buffer as part of a drawing command
		ACCESS_FLAG_VERTEX_READ = 0x00000004,

		/// Read access to a uniform buffer
		ACCESS_FLAG_UNIFORM_READ = 0x00000008,

		/// Read access to an input attachment within a render pass during fragment shading
		ACCESS_FLAG_INPUT_ATTACHMENT_READ = 0x00000010,

		/// Read access from a shader resource, formatted buffer, UAV
		ACCESS_FLAG_SHADER_READ = 0x00000020,

		/// Write access to a UAV
		ACCESS_FLAG_SHADER_WRITE = 0x00000040,

		/// Read access to a color render target, such as via blending,
		/// logic operations, or via certain subpass load operations.
		ACCESS_FLAG_RENDER_TARGET_READ = 0x00000080,

		/// Write access to a color render target, resolve, or depth/stencil resolve
		/// attachment during a render pass or via certain subpass load and store operations.
		ACCESS_FLAG_RENDER_TARGET_WRITE = 0x00000100,

		/// Read access to a depth/stencil buffer, via depth or stencil operations
		/// or via certain subpass load operations
		ACCESS_FLAG_DEPTH_STENCIL_READ = 0x00000200,

		/// Write access to a depth/stencil buffer, via depth or stencil operations
		/// or via certain subpass load and store operations
		ACCESS_FLAG_DEPTH_STENCIL_WRITE = 0x00000400,

		/// Read access to an texture or buffer in a copy operation.
		ACCESS_FLAG_COPY_SRC = 0x00000800,

		/// Write access to an texture or buffer in a copy operation.
		ACCESS_FLAG_COPY_DST = 0x00001000,

		/// Read access by a host operation. Accesses of this type are
		/// not performed through a resource, but directly on memory.
		ACCESS_FLAG_HOST_READ = 0x00002000,

		/// Write access by a host operation. Accesses of this type are
		/// not performed through a resource, but directly on memory.
		ACCESS_FLAG_HOST_WRITE = 0x00004000,

		/// All read accesses. It is always valid in any access mask,
		/// and is treated as equivalent to setting all READ access flags
		/// that are valid where it is used.
		ACCESS_FLAG_MEMORY_READ = 0x00008000,

		/// All write accesses. It is always valid in any access mask,
		/// and is treated as equivalent to setting all WRITE access
		// flags that are valid where it is used.
		ACCESS_FLAG_MEMORY_WRITE = 0x00010000,

		/// Read access to a predicate as part of conditional rendering.
		ACCESS_FLAG_CONDITIONAL_RENDERING_READ = 0x00100000,

		/// Read access to a shading rate texture as part of a drawing comman.
		ACCESS_FLAG_SHADING_RATE_TEXTURE_READ = 0x00800000,

		/// Read access to an acceleration structure as part of a trace or build command.
		ACCESS_FLAG_ACCELERATION_STRUCTURE_READ = 0x00200000,

		/// Write access to an acceleration structure or acceleration structure
		/// scratch buffer as part of a build command.
		ACCESS_FLAG_ACCELERATION_STRUCTURE_WRITE = 0x00400000,

		/// Read access to a fragment density map attachment during
		/// dynamic fragment density map operations.
		ACCESS_FLAG_FRAGMENT_DENSITY_MAP_READ = 0x01000000,

		/// Default access type that is determined by the resource state.
		/// For example, RESOURCE_STATE_RENDER_TARGET corresponds to
		/// ACCESS_FLAG_RENDER_TARGET_WRITE access type.
		ACCESS_FLAG_DEFAULT = 0x80000000
    };
}