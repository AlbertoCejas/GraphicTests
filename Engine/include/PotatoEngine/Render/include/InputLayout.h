#pragma once

#include <cstdint>
#include <PotatoEngine/Render/include/GraphicsTypes.h>
#include <string>

namespace potato
{
    static const uint32_t MAX_LAYOUT_ELEMENTS = 16;
    static const uint32_t LAYOUT_ELEMENT_AUTO_OFFSET = 0xFFFFFFFF;
    static const uint32_t LAYOUT_ELEMENT_AUTO_STRIDE = 0xFFFFFFFF;

    /// Input frequency
    enum InputElementFrequency
    {
        /// Frequency is undefined.
        INPUT_ELEMENT_FREQUENCY_UNDEFINED = 0,

        /// Input data is per-vertex data.
        INPUT_ELEMENT_FREQUENCY_PER_VERTEX,

        /// Input data is per-instance data.
        INPUT_ELEMENT_FREQUENCY_PER_INSTANCE,

        /// Helper value that stores the total number of frequencies in the enumeration.
        INPUT_ELEMENT_FREQUENCY_NUM_FREQUENCIES
    };

    /// Description of a single element of the input layout
    struct LayoutElement
    {
        /// HLSL semantic. Default value ("ATTRIB") allows HLSL shaders to be converted
        /// to GLSL and used in OpenGL backend as well as compiled to SPIRV and used
        /// in Vulkan backend.
        /// Any value other than default will only work in Direct3D11 and Direct3D12 backends.
        std::string HLSLSemantic = "ATTRIB";

        /// Input index of the element that is specified in the vertex shader.
        /// In Direct3D11 and Direct3D12 backends this is the semantic index.
        uint32_t inputIndex = 0;

        /// Buffer slot index that this element is read from.
        uint32_t bufferSlot = 0;

        /// Number of components in the element. Allowed values are 1, 2, 3, and 4.
        uint32_t numComponents = 0;

        /// Type of the element components, see Diligent::VALUE_TYPE for details.
        ValueType valueType = ValueType::VT_FLOAT32;

        /// For signed and unsigned integer value types 
        /// (VT_INT8, VT_INT16, VT_INT32, VT_UINT8, VT_UINT16, VT_UINT32)
        /// indicates if the value should be normalized to [-1,+1] or 
        /// [0, 1] range respectively. For floating point types
        /// (VT_FLOAT16 and VT_FLOAT32), this member is ignored.
        bool isNormalized = true;

        /// Relative offset, in bytes, to the element bits.
        /// If this value is set to LAYOUT_ELEMENT_AUTO_OFFSET (default value), the offset will
        /// be computed automatically by placing the element right after the previous one.
        uint32_t relativeOffset = LAYOUT_ELEMENT_AUTO_OFFSET;

        /// Stride, in bytes, between two elements, for this buffer slot.
        /// If this value is set to LAYOUT_ELEMENT_AUTO_STRIDE, the stride will be
        /// computed automatically assuming that all elements in the same buffer slot are
        /// packed one after another. If the buffer slot contains multiple layout elements,
        /// they all must specify the same stride or use LAYOUT_ELEMENT_AUTO_STRIDE value.
        uint32_t stride = LAYOUT_ELEMENT_AUTO_STRIDE;

        enum InputElementFrequency frequency = InputElementFrequency::INPUT_ELEMENT_FREQUENCY_PER_VERTEX;

        /// The number of instances to draw using the same per-instance data before advancing 
        /// in the buffer by one element.
        //Uint32 InstanceDataStepRate DEFAULT_INITIALIZER(1);
    };

    /// Layout description
    /// This structure is used by IRenderDevice::CreateGraphicsPipelineState().
    struct InputLayoutDesc
    {
        /// Array of layout elements
        const LayoutElement* layoutElements = nullptr;
        /// Number of layout elements
        uint32_t numElements = 0;
    }
}