#include <Core/include/Utils/VKConversions.h>

#include <cstdint>
#include <PotatoEngine/Render/include/InputLayout.h>
#include <PotatoEngine/Util/Assert.h>

using namespace potato;
using namespace potato::vk;


class TexFormatToVkFormatMapper
{
    public:
        TexFormatToVkFormatMapper()
        {
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_UNKNOWN] = VK_FORMAT_UNDEFINED;

            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGBA32_TYPELESS] = VK_FORMAT_R32G32B32A32_SFLOAT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGBA32_FLOAT] = VK_FORMAT_R32G32B32A32_SFLOAT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGBA32_UINT] = VK_FORMAT_R32G32B32A32_UINT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGBA32_SINT] = VK_FORMAT_R32G32B32A32_SINT;

            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGB32_TYPELESS] = VK_FORMAT_R32G32B32_SFLOAT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGB32_FLOAT] = VK_FORMAT_R32G32B32_SFLOAT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGB32_UINT] = VK_FORMAT_R32G32B32_UINT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGB32_SINT] = VK_FORMAT_R32G32B32_SINT;

            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGBA16_TYPELESS] = VK_FORMAT_R16G16B16A16_SFLOAT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGBA16_FLOAT] = VK_FORMAT_R16G16B16A16_SFLOAT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGBA16_UNORM] = VK_FORMAT_R16G16B16A16_UNORM;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGBA16_UINT] = VK_FORMAT_R16G16B16A16_UINT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGBA16_SNORM] = VK_FORMAT_R16G16B16A16_SNORM;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGBA16_SINT] = VK_FORMAT_R16G16B16A16_SINT;

            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RG32_TYPELESS] = VK_FORMAT_R32G32_SFLOAT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RG32_FLOAT] = VK_FORMAT_R32G32_SFLOAT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RG32_UINT] = VK_FORMAT_R32G32_UINT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RG32_SINT] = VK_FORMAT_R32G32_SINT;

            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_R32G8X24_TYPELESS] = VK_FORMAT_D32_SFLOAT_S8_UINT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_D32_FLOAT_S8X24_UINT] = VK_FORMAT_D32_SFLOAT_S8_UINT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_R32_FLOAT_X8X24_TYPELESS] = VK_FORMAT_D32_SFLOAT_S8_UINT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_X32_TYPELESS_G8X24_UINT] = VK_FORMAT_UNDEFINED;

            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGB10A2_TYPELESS] = VK_FORMAT_A2R10G10B10_UNORM_PACK32;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGB10A2_UNORM] = VK_FORMAT_A2R10G10B10_UNORM_PACK32;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGB10A2_UINT] = VK_FORMAT_A2R10G10B10_UINT_PACK32;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_R11G11B10_FLOAT] = VK_FORMAT_B10G11R11_UFLOAT_PACK32;

            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGBA8_TYPELESS] = VK_FORMAT_R8G8B8A8_UNORM;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGBA8_UNORM] = VK_FORMAT_R8G8B8A8_UNORM;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGBA8_UNORM_SRGB] = VK_FORMAT_R8G8B8A8_SRGB;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGBA8_UINT] = VK_FORMAT_R8G8B8A8_UINT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGBA8_SNORM] = VK_FORMAT_R8G8B8A8_SNORM;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGBA8_SINT] = VK_FORMAT_R8G8B8A8_SINT;

            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RG16_TYPELESS] = VK_FORMAT_R16G16_SFLOAT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RG16_FLOAT] = VK_FORMAT_R16G16_SFLOAT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RG16_UNORM] = VK_FORMAT_R16G16_UNORM;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RG16_UINT] = VK_FORMAT_R16G16_UINT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RG16_SNORM] = VK_FORMAT_R16G16_SNORM;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RG16_SINT] = VK_FORMAT_R16G16_SINT;

            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_R32_TYPELESS] = VK_FORMAT_R32_SFLOAT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_D32_FLOAT] = VK_FORMAT_D32_SFLOAT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_R32_FLOAT] = VK_FORMAT_R32_SFLOAT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_R32_UINT] = VK_FORMAT_R32_UINT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_R32_SINT] = VK_FORMAT_R32_SINT;

            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_R24G8_TYPELESS] = VK_FORMAT_D24_UNORM_S8_UINT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_D24_UNORM_S8_UINT] = VK_FORMAT_D24_UNORM_S8_UINT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_R24_UNORM_X8_TYPELESS] = VK_FORMAT_D24_UNORM_S8_UINT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_X24_TYPELESS_G8_UINT] = VK_FORMAT_UNDEFINED;

            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RG8_TYPELESS] = VK_FORMAT_R8G8_UNORM;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RG8_UNORM] = VK_FORMAT_R8G8_UNORM;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RG8_UINT] = VK_FORMAT_R8G8_UINT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RG8_SNORM] = VK_FORMAT_R8G8_SNORM;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RG8_SINT] = VK_FORMAT_R8G8_SINT;

            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_R16_TYPELESS] = VK_FORMAT_R16_SFLOAT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_R16_FLOAT] = VK_FORMAT_R16_SFLOAT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_D16_UNORM] = VK_FORMAT_D16_UNORM;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_R16_UNORM] = VK_FORMAT_R16_UNORM;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_R16_UINT] = VK_FORMAT_R16_UINT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_R16_SNORM] = VK_FORMAT_R16_SNORM;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_R16_SINT] = VK_FORMAT_R16_SINT;

            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_R8_TYPELESS] = VK_FORMAT_R8_UNORM;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_R8_UNORM] = VK_FORMAT_R8_UNORM;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_R8_UINT] = VK_FORMAT_R8_UINT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_R8_SNORM] = VK_FORMAT_R8_SNORM;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_R8_SINT] = VK_FORMAT_R8_SINT;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_A8_UNORM] = VK_FORMAT_UNDEFINED;

            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_R1_UNORM] = VK_FORMAT_UNDEFINED;

            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RGB9E5_SHAREDEXP] = VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_RG8_B8G8_UNORM] = VK_FORMAT_UNDEFINED;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_G8R8_G8B8_UNORM] = VK_FORMAT_UNDEFINED;

            // http://www.g-truc.net/post-0335.html
            // http://renderingpipeline.com/2012/07/texture-compression/
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BC1_TYPELESS] = VK_FORMAT_BC1_RGB_UNORM_BLOCK;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BC1_UNORM] = VK_FORMAT_BC1_RGB_UNORM_BLOCK;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BC1_UNORM_SRGB] = VK_FORMAT_BC1_RGB_SRGB_BLOCK;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BC2_TYPELESS] = VK_FORMAT_BC2_UNORM_BLOCK;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BC2_UNORM] = VK_FORMAT_BC2_UNORM_BLOCK;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BC2_UNORM_SRGB] = VK_FORMAT_BC2_SRGB_BLOCK;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BC3_TYPELESS] = VK_FORMAT_BC3_UNORM_BLOCK;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BC3_UNORM] = VK_FORMAT_BC3_UNORM_BLOCK;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BC3_UNORM_SRGB] = VK_FORMAT_BC3_SRGB_BLOCK;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BC4_TYPELESS] = VK_FORMAT_BC4_UNORM_BLOCK;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BC4_UNORM] = VK_FORMAT_BC4_UNORM_BLOCK;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BC4_SNORM] = VK_FORMAT_BC4_SNORM_BLOCK;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BC5_TYPELESS] = VK_FORMAT_BC5_UNORM_BLOCK;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BC5_UNORM] = VK_FORMAT_BC5_UNORM_BLOCK;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BC5_SNORM] = VK_FORMAT_BC5_SNORM_BLOCK;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_B5G6R5_UNORM] = VK_FORMAT_B5G6R5_UNORM_PACK16;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_B5G5R5A1_UNORM] = VK_FORMAT_B5G5R5A1_UNORM_PACK16;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BGRA8_UNORM] = VK_FORMAT_B8G8R8A8_UNORM;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BGRX8_UNORM] = VK_FORMAT_B8G8R8A8_UNORM;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_R10G10B10_XR_BIAS_A2_UNORM] = VK_FORMAT_UNDEFINED;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BGRA8_TYPELESS] = VK_FORMAT_B8G8R8A8_UNORM;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BGRA8_UNORM_SRGB] = VK_FORMAT_B8G8R8A8_SRGB;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BGRX8_TYPELESS] = VK_FORMAT_B8G8R8A8_UNORM;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BGRX8_UNORM_SRGB] = VK_FORMAT_B8G8R8A8_SRGB;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BC6H_TYPELESS] = VK_FORMAT_BC6H_UFLOAT_BLOCK;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BC6H_UF16] = VK_FORMAT_BC6H_UFLOAT_BLOCK;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BC6H_SF16] = VK_FORMAT_BC6H_SFLOAT_BLOCK;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BC7_TYPELESS] = VK_FORMAT_BC7_UNORM_BLOCK;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BC7_UNORM] = VK_FORMAT_BC7_UNORM_BLOCK;
            m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_BC7_UNORM_SRGB] = VK_FORMAT_BC7_SRGB_BLOCK;
        }

        VkFormat operator[](TextureFormat texFmt) const
        {
            POTATO_ASSERT(texFmt < std::size(m_textureFormatToVKMap));
            return m_textureFormatToVKMap[texFmt];
        }

    private:
        VkFormat m_textureFormatToVKMap[TextureFormat::TEX_FORMAT_NUM_FORMATS] = {};
};

VkFormat potato::vk::textureFormatToVkFormat(TextureFormat textureFormat)
{
    static const TexFormatToVkFormatMapper textureFormatVKMapper;
    return textureFormatVKMapper[textureFormat];
}

static_assert(AttachmentLoadOp::ATTACHMENT_LOAD_OP_LOAD == VK_ATTACHMENT_LOAD_OP_LOAD);
static_assert(AttachmentLoadOp::ATTACHMENT_LOAD_OP_CLEAR == VK_ATTACHMENT_LOAD_OP_CLEAR);
static_assert(AttachmentLoadOp::ATTACHMENT_LOAD_OP_DISCARD == VK_ATTACHMENT_LOAD_OP_DONT_CARE);
VkAttachmentLoadOp potato::vk::attachmentLoadOpToVkAttachmentLoadOp(AttachmentLoadOp LoadOp)
{
    return static_cast<VkAttachmentLoadOp>(LoadOp);
}

static_assert(AttachmentStoreOp::ATTACHMENT_STORE_OP_STORE == VK_ATTACHMENT_STORE_OP_STORE);
static_assert(AttachmentStoreOp::ATTACHMENT_STORE_OP_DISCARD == VK_ATTACHMENT_STORE_OP_DONT_CARE);
VkAttachmentStoreOp potato::vk::attachmentStoreOpToVkAttachmentStoreOp(AttachmentStoreOp StoreOp)
{
    return static_cast<VkAttachmentStoreOp>(StoreOp);
}

VkImageLayout potato::vk::resourceStateToVkImageLayout(ResourceState stateFlag, bool isInsideRenderPass = false)
{
    if (stateFlag == ResourceState::RESOURCE_STATE_UNKNOWN)
    {
        return VK_IMAGE_LAYOUT_UNDEFINED;
    }
    // Currently not used:
    //VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL
    //VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL
    //VK_IMAGE_LAYOUT_SHADING_RATE_OPTIMAL_NV
    //VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL,
    //VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL,

    static_assert(RESOURCE_STATE_MAX_BIT == RESOURCE_STATE_RAY_TRACING, "This function must be updated to handle new resource state flag");
    POTATO_ASSERT_MSG((stateFlag & (stateFlag - 1)) == 0, "Only single bit must be set");
    switch (stateFlag)
    {
        case RESOURCE_STATE_UNDEFINED:         return VK_IMAGE_LAYOUT_UNDEFINED;
        case RESOURCE_STATE_VERTEX_BUFFER:     /*UNEXPECTED("Invalid resource state");*/ return VK_IMAGE_LAYOUT_UNDEFINED;
        case RESOURCE_STATE_CONSTANT_BUFFER:   /*UNEXPECTED("Invalid resource state"); */ return VK_IMAGE_LAYOUT_UNDEFINED;
        case RESOURCE_STATE_INDEX_BUFFER:      /*UNEXPECTED("Invalid resource state"); */ return VK_IMAGE_LAYOUT_UNDEFINED;
        case RESOURCE_STATE_RENDER_TARGET:     return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        case RESOURCE_STATE_UNORDERED_ACCESS:  return VK_IMAGE_LAYOUT_GENERAL;
        case RESOURCE_STATE_DEPTH_WRITE:       return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        case RESOURCE_STATE_DEPTH_READ:        return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
        case RESOURCE_STATE_SHADER_RESOURCE:   return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        case RESOURCE_STATE_STREAM_OUT:        /*UNEXPECTED("Invalid resource state");*/ return VK_IMAGE_LAYOUT_UNDEFINED;
        case RESOURCE_STATE_INDIRECT_ARGUMENT: /*UNEXPECTED("Invalid resource state");*/ return VK_IMAGE_LAYOUT_UNDEFINED;
        case RESOURCE_STATE_COPY_DEST:         return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        case RESOURCE_STATE_COPY_SOURCE:       return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        case RESOURCE_STATE_RESOLVE_DEST:      return isInsideRenderPass ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        case RESOURCE_STATE_RESOLVE_SOURCE:    return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        case RESOURCE_STATE_INPUT_ATTACHMENT:  return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        case RESOURCE_STATE_PRESENT:           return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        case RESOURCE_STATE_BUILD_AS_READ:     /*UNEXPECTED("Invalid resource state");*/ return VK_IMAGE_LAYOUT_UNDEFINED;
        case RESOURCE_STATE_BUILD_AS_WRITE:    /*UNEXPECTED("Invalid resource state");*/ return VK_IMAGE_LAYOUT_UNDEFINED;
        case RESOURCE_STATE_RAY_TRACING:       /*UNEXPECTED("Invalid resource state");*/ return VK_IMAGE_LAYOUT_UNDEFINED;

        default:
            /*UNEXPECTED("Unexpected resource state flag (", stateFlag, ")");*/
            return VK_IMAGE_LAYOUT_UNDEFINED;
    }
}

VkShaderStageFlagBits potato::vk::shaderTypeToVkShaderStageFlagBit(ShaderType shaderType)
{
    static_assert(SHADER_TYPE_LAST == SHADER_TYPE_CALLABLE, "Please update the switch below to handle the new shader type");
    // TODO:
    // POTATO_ASSERT(IsPowerOfTwo(Uint32{ ShaderType }), "More than one shader type is specified");
    switch (shaderType)
    {
        case SHADER_TYPE_VERTEX:           return VK_SHADER_STAGE_VERTEX_BIT;
        case SHADER_TYPE_HULL:             return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        case SHADER_TYPE_DOMAIN:           return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        case SHADER_TYPE_GEOMETRY:         return VK_SHADER_STAGE_GEOMETRY_BIT;
        case SHADER_TYPE_PIXEL:            return VK_SHADER_STAGE_FRAGMENT_BIT;
        case SHADER_TYPE_COMPUTE:          return VK_SHADER_STAGE_COMPUTE_BIT;
        case SHADER_TYPE_AMPLIFICATION:    return VK_SHADER_STAGE_TASK_BIT_NV;
        case SHADER_TYPE_MESH:             return VK_SHADER_STAGE_MESH_BIT_NV;
        case SHADER_TYPE_RAY_GEN:          return VK_SHADER_STAGE_RAYGEN_BIT_KHR;
        case SHADER_TYPE_RAY_MISS:         return VK_SHADER_STAGE_MISS_BIT_KHR;
        case SHADER_TYPE_RAY_CLOSEST_HIT:  return VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
        case SHADER_TYPE_RAY_ANY_HIT:      return VK_SHADER_STAGE_ANY_HIT_BIT_KHR;
        case SHADER_TYPE_RAY_INTERSECTION: return VK_SHADER_STAGE_INTERSECTION_BIT_KHR;
        case SHADER_TYPE_CALLABLE:         return VK_SHADER_STAGE_CALLABLE_BIT_KHR;
        default:
            //UNEXPECTED("Unknown shader type");
            return VK_SHADER_STAGE_VERTEX_BIT;
    }
}

VkVertexInputRate potato::vk::layoutElemFrequencyToVkInputRate(InputElementFrequency frequency)
{
    switch (frequency)
    {
        case INPUT_ELEMENT_FREQUENCY_UNDEFINED:
            //UNEXPECTED("Undefined layout element frequency");
            return VK_VERTEX_INPUT_RATE_VERTEX;

        case INPUT_ELEMENT_FREQUENCY_PER_VERTEX:
            return VK_VERTEX_INPUT_RATE_VERTEX;
        case INPUT_ELEMENT_FREQUENCY_PER_INSTANCE:
            return VK_VERTEX_INPUT_RATE_INSTANCE;

        default:
            //UNEXPECTED("Unknown layout element frequency");
            return VK_VERTEX_INPUT_RATE_VERTEX;
    }
}

VkFormat potato::vk::typeToVkFormat(ValueType valType, uint32_t numComponents, bool isNormalized)
{
    switch (valType)
    {
        case VT_FLOAT16:
        {
            POTATO_ASSERT_MSG(!isNormalized, "Floating point formats cannot be normalized");
            switch (numComponents)
            {
                case 1: return VK_FORMAT_R16_SFLOAT;
                case 2: return VK_FORMAT_R16G16_SFLOAT;
                case 3: return VK_FORMAT_R16G16B16_SFLOAT;
                case 4: return VK_FORMAT_R16G16B16A16_SFLOAT;
                default: 
                    //UNEXPECTED("Unusupported number of components"); 
                    return VK_FORMAT_UNDEFINED;
            }
        }

        case VT_FLOAT32:
        {
            POTATO_ASSERT_MSG(!isNormalized, "Floating point formats cannot be normalized");
            switch (numComponents)
            {
                case 1: return VK_FORMAT_R32_SFLOAT;
                case 2: return VK_FORMAT_R32G32_SFLOAT;
                case 3: return VK_FORMAT_R32G32B32_SFLOAT;
                case 4: return VK_FORMAT_R32G32B32A32_SFLOAT;
                default: 
                    //UNEXPECTED("Unusupported number of components"); 
                    return VK_FORMAT_UNDEFINED;
            }
        }

        case VT_INT32:
        {
            POTATO_ASSERT_MSG(!isNormalized, "32-bit UNORM formats are not supported. Use R32_FLOAT instead");
            switch (numComponents)
            {
                case 1: return VK_FORMAT_R32_SINT;
                case 2: return VK_FORMAT_R32G32_SINT;
                case 3: return VK_FORMAT_R32G32B32_SINT;
                case 4: return VK_FORMAT_R32G32B32A32_SINT;
                default: 
                    //UNEXPECTED("Unusupported number of components"); 
                    return VK_FORMAT_UNDEFINED;
            }
        }

        case VT_UINT32:
        {
            POTATO_ASSERT_MSG(!isNormalized, "32-bit UNORM formats are not supported. Use R32_FLOAT instead");
            switch (numComponents)
            {
                case 1: return VK_FORMAT_R32_UINT;
                case 2: return VK_FORMAT_R32G32_UINT;
                case 3: return VK_FORMAT_R32G32B32_UINT;
                case 4: return VK_FORMAT_R32G32B32A32_UINT;
                default: 
                    //UNEXPECTED("Unusupported number of components"); 
                    return VK_FORMAT_UNDEFINED;
            }
        }

        case VT_INT16:
        {
            if (isNormalized)
            {
                switch (numComponents)
                {
                    case 1: return VK_FORMAT_R16_SNORM;
                    case 2: return VK_FORMAT_R16G16_SNORM;
                    case 3: return VK_FORMAT_R16G16B16_SNORM;
                    case 4: return VK_FORMAT_R16G16B16A16_SNORM;
                    default: 
                        //UNEXPECTED("Unusupported number of components"); 
                        return VK_FORMAT_UNDEFINED;
                }
            }
            else
            {
                switch (numComponents)
                {
                    case 1: return VK_FORMAT_R16_SINT;
                    case 2: return VK_FORMAT_R16G16_SINT;
                    case 3: return VK_FORMAT_R16G16B16_SINT;
                    case 4: return VK_FORMAT_R16G16B16A16_SINT;
                    default: 
                        //UNEXPECTED("Unusupported number of components"); 
                        return VK_FORMAT_UNDEFINED;
                }
            }
        }

        case VT_UINT16:
        {
            if (isNormalized)
            {
                switch (numComponents)
                {
                    case 1: return VK_FORMAT_R16_UNORM;
                    case 2: return VK_FORMAT_R16G16_UNORM;
                    case 3: return VK_FORMAT_R16G16B16_UNORM;
                    case 4: return VK_FORMAT_R16G16B16A16_UNORM;
                    default: 
                        //UNEXPECTED("Unusupported number of components"); 
                        return VK_FORMAT_UNDEFINED;
                }
            }
            else
            {
                switch (numComponents)
                {
                    case 1: return VK_FORMAT_R16_UINT;
                    case 2: return VK_FORMAT_R16G16_UINT;
                    case 3: return VK_FORMAT_R16G16B16_UINT;
                    case 4: return VK_FORMAT_R16G16B16A16_UINT;
                    default: 
                        //UNEXPECTED("Unusupported number of components"); 
                        return VK_FORMAT_UNDEFINED;
                }
            }
        }

        case VT_INT8:
        {
            if (isNormalized)
            {
                switch (numComponents)
                {
                    case 1: return VK_FORMAT_R8_SNORM;
                    case 2: return VK_FORMAT_R8G8_SNORM;
                    case 3: return VK_FORMAT_R8G8B8_SNORM;
                    case 4: return VK_FORMAT_R8G8B8A8_SNORM;
                    default: 
                        //UNEXPECTED("Unusupported number of components"); 
                        return VK_FORMAT_UNDEFINED;
                }
            }
            else
            {
                switch (numComponents)
                {
                    case 1: return VK_FORMAT_R8_SINT;
                    case 2: return VK_FORMAT_R8G8_SINT;
                    case 3: return VK_FORMAT_R8G8B8_SINT;
                    case 4: return VK_FORMAT_R8G8B8A8_SINT;
                    default: 
                        //UNEXPECTED("Unusupported number of components"); 
                        return VK_FORMAT_UNDEFINED;
                }
            }
        }

        case VT_UINT8:
        {
            if (isNormalized)
            {
                switch (numComponents)
                {
                    case 1: return VK_FORMAT_R8_UNORM;
                    case 2: return VK_FORMAT_R8G8_UNORM;
                    case 3: return VK_FORMAT_R8G8B8_UNORM;
                    case 4: return VK_FORMAT_R8G8B8A8_UNORM;
                    default: 
                        //UNEXPECTED("Unusupported number of components"); 
                        return VK_FORMAT_UNDEFINED;
                }
            }
            else
            {
                switch (numComponents)
                {
                    case 1: return VK_FORMAT_R8_UINT;
                    case 2: return VK_FORMAT_R8G8_UINT;
                    case 3: return VK_FORMAT_R8G8B8_UINT;
                    case 4: return VK_FORMAT_R8G8B8A8_UINT;
                    default: 
                        //UNEXPECTED("Unusupported number of components"); 
                        return VK_FORMAT_UNDEFINED;
                }
            }
        }

        default: 
            //UNEXPECTED("Unusupported format"); 
            return VK_FORMAT_UNDEFINED;
    }
}

void potato::vk::inputLayoutDescToVkVertexInputStateCI(const InputLayoutDesc& layoutDesc,
    VkPipelineVertexInputStateCreateInfo& vertexInputStateCI,
    std::array<VkVertexInputBindingDescription, MAX_LAYOUT_ELEMENTS>& bindingDescriptions,
    std::array<VkVertexInputAttributeDescription, MAX_LAYOUT_ELEMENTS>& attributeDescription)
{
    // Vertex input description (20.2)
    vertexInputStateCI.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputStateCI.pNext = nullptr;
    vertexInputStateCI.flags = 0; // reserved for future use.
    vertexInputStateCI.vertexBindingDescriptionCount = 0;
    vertexInputStateCI.pVertexBindingDescriptions = bindingDescriptions.data();
    vertexInputStateCI.vertexAttributeDescriptionCount = layoutDesc.numElements;
    vertexInputStateCI.pVertexAttributeDescriptions = attributeDescription.data();

    
    std::array<int32_t, MAX_LAYOUT_ELEMENTS> bufferSlot2BindingDescInd;
    bufferSlot2BindingDescInd.fill(-1);
    for (uint32_t elem = 0; elem < layoutDesc.numElements; ++elem)
    {
        auto& layoutElem = layoutDesc.layoutElements[elem];
        auto& bindingDescInd = bufferSlot2BindingDescInd[layoutElem.bufferSlot];
        if (bindingDescInd < 0)
        {
            bindingDescInd = vertexInputStateCI.vertexBindingDescriptionCount++;
            auto& bindingDesc = bindingDescriptions[bindingDescInd];
            bindingDesc.binding = layoutElem.bufferSlot;
            bindingDesc.stride = layoutElem.stride;
            bindingDesc.inputRate = layoutElemFrequencyToVkInputRate(layoutElem.frequency);
        }

        const auto& bindingDesc = bindingDescriptions[bindingDescInd];
        POTATO_ASSERT_MSG(bindingDesc.binding == layoutElem.bufferSlot, "Inconsistent buffer slot");
        POTATO_ASSERT_MSG(bindingDesc.stride == layoutElem.stride, "Inconsistent strides");
        POTATO_ASSERT_MSG(bindingDesc.inputRate == layoutElemFrequencyToVkInputRate(layoutElem.frequency), "Incosistent layout element frequency");

        auto& attribDesc = attributeDescription[elem];
        attribDesc.binding = bindingDesc.binding;
        attribDesc.location = layoutElem.inputIndex;
        attribDesc.format = typeToVkFormat(layoutElem.valueType, layoutElem.numComponents, layoutElem.isNormalized);
        attribDesc.offset = layoutElem.relativeOffset;
    }
}