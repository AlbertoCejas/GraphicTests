#pragma once

#include <cstdint>
#include <PotatoEngine/Render/include/GraphicsTypes.h>
#include <PotatoEngine/Render/interface/IPipelineState.h>
#include <PotatoEngine/Render/interface/IShader.h>
#include <PotatoEngine/Util/Assert.h>
#include <string>

namespace potato
{
    /// Template structure to convert ValueType enumeration into C-type
    template <ValueType ValType>
    struct ValueType2CType
    {};

    /// ValueType2CType<> template specialization for 8-bit integer value type.

    /// Usage example:
    ///
    ///     ValueType2CType<VT_INT8>::CType MyInt8Var;
    template <> struct ValueType2CType<VT_INT8>
    {
        typedef int8_t CType;
    };

    /// ValueType2CType<> template specialization for 16-bit integer value type.

    /// Usage example:
    ///
    ///     ValueType2CType<VT_INT16>::CType MyInt16Var;
    template <> struct ValueType2CType<VT_INT16>
    {
        typedef int16_t CType;
    };

    /// ValueType2CType<> template specialization for 32-bit integer value type.

    /// Usage example:
    ///
    ///     ValueType2CType<VT_INT32>::CType MyInt32Var;
    template <> struct ValueType2CType<VT_INT32>
    {
        typedef int32_t CType;
    };

    /// ValueType2CType<> template specialization for 8-bit unsigned-integer value type.

    /// Usage example:
    ///
    ///     ValueType2CType<VT_UINT8>::CType MyUint8Var;
    template <> struct ValueType2CType<VT_UINT8>
    {
        typedef uint8_t CType;
    };

    /// ValueType2CType<> template specialization for 16-bit unsigned-integer value type.

    /// Usage example:
    ///
    ///     ValueType2CType<VT_UINT16>::CType MyUint16Var;
    template <> struct ValueType2CType<VT_UINT16>
    {
        typedef uint16_t CType;
    };

    /// ValueType2CType<> template specialization for 32-bit unsigned-integer value type.

    /// Usage example:
    ///
    ///     ValueType2CType<VT_UINT32>::CType MyUint32Var;
    template <> struct ValueType2CType<VT_UINT32>
    {
        typedef uint32_t CType;
    };

    /// ValueType2CType<> template specialization for half-precision 16-bit floating-point value type.

    /// Usage example:
    ///
    ///     ValueType2CType<VT_FLOAT16>::CType MyFloat16Var;
    ///
    /// \note 16-bit floating-point values have no corresponding C++ type and are translated to Uint16
    template <> struct ValueType2CType<VT_FLOAT16>
    {
        typedef uint16_t CType;
    };

    /// ValueType2CType<> template specialization for full-precision 32-bit floating-point value type.

    /// Usage example:
    ///
    ///     ValueType2CType<VT_FLOAT32>::CType MyFloat32Var;
    template <> struct ValueType2CType<VT_FLOAT32>
    {
        typedef float CType;
    };


    static const uint32_t valueTypeToSizeMap[] =
    {
        0,
        sizeof(ValueType2CType<VT_INT8>    ::CType),
        sizeof(ValueType2CType<VT_INT16>   ::CType),
        sizeof(ValueType2CType<VT_INT32>   ::CType),
        sizeof(ValueType2CType<VT_UINT8>   ::CType),
        sizeof(ValueType2CType<VT_UINT16>  ::CType),
        sizeof(ValueType2CType<VT_UINT32>  ::CType),
        sizeof(ValueType2CType<VT_FLOAT16> ::CType),
        sizeof(ValueType2CType<VT_FLOAT32> ::CType)
    };
    static_assert(VT_NUM_TYPES == VT_NUM_TYPES, "Not all value type sizes initialized.");

    /// Returns the size of the specified value type
    inline uint32_t getValueSize(ValueType val)
    {
        POTATO_ASSERT(val < std::size(valueTypeToSizeMap));
        return valueTypeToSizeMap[val];
    }

    int32_t getShaderTypePipelineIndex(ShaderType shaderType, PipelineType pipelineType)
    {
        //TODO
        /*VERIFY(IsConsistentShaderType(ShaderType, PipelineType), "Shader type ", GetShaderTypeLiteralName(ShaderType),
            " is inconsistent with pipeline type ", GetPipelineTypeString(PipelineType));
        VERIFY(IsPowerOfTwo(Uint32{ ShaderType }), "More than one shader type is specified");*/

        static_assert(SHADER_TYPE_LAST == SHADER_TYPE_CALLABLE, "Please update the switch below to handle the new shader type");
        switch (shaderType)
        {
        case SHADER_TYPE_UNKNOWN:
            return -1;

        case SHADER_TYPE_VERTEX:        // Graphics
        case SHADER_TYPE_AMPLIFICATION: // Mesh
        case SHADER_TYPE_COMPUTE:       // Compute
        case SHADER_TYPE_RAY_GEN:       // Ray tracing
            return 0;

        case SHADER_TYPE_HULL:     // Graphics
        case SHADER_TYPE_MESH:     // Mesh
        case SHADER_TYPE_RAY_MISS: // RayTracing
            return 1;

        case SHADER_TYPE_DOMAIN:          // Graphics
        case SHADER_TYPE_RAY_CLOSEST_HIT: // Ray tracing
            return 2;

        case SHADER_TYPE_GEOMETRY:    // Graphics
        case SHADER_TYPE_RAY_ANY_HIT: // Ray tracing
            return 3;

        case SHADER_TYPE_PIXEL:            // Graphics or Mesh
        case SHADER_TYPE_RAY_INTERSECTION: // Ray tracing
            return 4;

        case SHADER_TYPE_CALLABLE: // RayTracing
            return 5;

        default:
            //UNEXPECTED("Unexpected shader type (", shaderType, ")");
            return -1;
        }
    }

}