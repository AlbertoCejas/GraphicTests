#include <PotatoImGui/ImGuiPotatoRenderer.h>

#include <PotatoEngine/Util/Assert.h>
#include <PotatoEngine/Render/DeviceCapabilities.h>
#include <PotatoEngine/Render/IPipelineState.h>
#include <PotatoEngine/Render/IRenderDevice.h>
#include <PotatoEngine/Render/IShader.h>
#include <PotatoEngine/Render/RasterizerState.h>

#include <imgui.h>

#include <stdlib.h>

namespace potato
{
    static constexpr uint32_t vertexShader_SPIRV[] =
    {
        0x07230203,0x00010000,0x0008000a,0x00000028,0x00000000,0x00020011,0x00000001,0x0006000b,
        0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
        0x000b000f,0x00000000,0x00000004,0x6e69616d,0x00000000,0x0000000a,0x00000016,0x00000020,
        0x00000022,0x00000025,0x00000026,0x00030003,0x00000002,0x000001a4,0x00040005,0x00000004,
        0x6e69616d,0x00000000,0x00060005,0x00000008,0x505f6c67,0x65567265,0x78657472,0x00000000,
        0x00060006,0x00000008,0x00000000,0x505f6c67,0x7469736f,0x006e6f69,0x00030005,0x0000000a,
        0x00000000,0x00050005,0x0000000e,0x736e6f43,0x746e6174,0x00000073,0x00080006,0x0000000e,
        0x00000000,0x6a6f7250,0x69746365,0x614d6e6f,0x78697274,0x00000000,0x00030005,0x00000010,
        0x00000000,0x00040005,0x00000016,0x705f6e69,0x0000736f,0x00050005,0x00000020,0x756f7376,
        0x6f635f74,0x0000006c,0x00040005,0x00000022,0x635f6e69,0x00006c6f,0x00050005,0x00000025,
        0x756f7376,0x76755f74,0x00000000,0x00040005,0x00000026,0x755f6e69,0x00000076,0x00050048,
        0x00000008,0x00000000,0x0000000b,0x00000000,0x00030047,0x00000008,0x00000002,0x00040048,
        0x0000000e,0x00000000,0x00000005,0x00050048,0x0000000e,0x00000000,0x00000023,0x00000000,
        0x00050048,0x0000000e,0x00000000,0x00000007,0x00000010,0x00030047,0x0000000e,0x00000002,
        0x00040047,0x00000010,0x00000022,0x00000000,0x00040047,0x00000010,0x00000021,0x00000000,
        0x00040047,0x00000016,0x0000001e,0x00000000,0x00040047,0x00000020,0x0000001e,0x00000000,
        0x00040047,0x00000022,0x0000001e,0x00000002,0x00040047,0x00000025,0x0000001e,0x00000001,
        0x00040047,0x00000026,0x0000001e,0x00000001,0x00020013,0x00000002,0x00030021,0x00000003,
        0x00000002,0x00030016,0x00000006,0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,
        0x0003001e,0x00000008,0x00000007,0x00040020,0x00000009,0x00000003,0x00000008,0x0004003b,
        0x00000009,0x0000000a,0x00000003,0x00040015,0x0000000b,0x00000020,0x00000001,0x0004002b,
        0x0000000b,0x0000000c,0x00000000,0x00040018,0x0000000d,0x00000007,0x00000004,0x0003001e,
        0x0000000e,0x0000000d,0x00040020,0x0000000f,0x00000002,0x0000000e,0x0004003b,0x0000000f,
        0x00000010,0x00000002,0x00040020,0x00000011,0x00000002,0x0000000d,0x00040017,0x00000014,
        0x00000006,0x00000002,0x00040020,0x00000015,0x00000001,0x00000014,0x0004003b,0x00000015,
        0x00000016,0x00000001,0x0004002b,0x00000006,0x00000018,0x00000000,0x0004002b,0x00000006,
        0x00000019,0x3f800000,0x00040020,0x0000001e,0x00000003,0x00000007,0x0004003b,0x0000001e,
        0x00000020,0x00000003,0x00040020,0x00000021,0x00000001,0x00000007,0x0004003b,0x00000021,
        0x00000022,0x00000001,0x00040020,0x00000024,0x00000003,0x00000014,0x0004003b,0x00000024,
        0x00000025,0x00000003,0x0004003b,0x00000015,0x00000026,0x00000001,0x00050036,0x00000002,
        0x00000004,0x00000000,0x00000003,0x000200f8,0x00000005,0x00050041,0x00000011,0x00000012,
        0x00000010,0x0000000c,0x0004003d,0x0000000d,0x00000013,0x00000012,0x0004003d,0x00000014,
        0x00000017,0x00000016,0x00050051,0x00000006,0x0000001a,0x00000017,0x00000000,0x00050051,
        0x00000006,0x0000001b,0x00000017,0x00000001,0x00070050,0x00000007,0x0000001c,0x0000001a,
        0x0000001b,0x00000018,0x00000019,0x00050091,0x00000007,0x0000001d,0x00000013,0x0000001c,
        0x00050041,0x0000001e,0x0000001f,0x0000000a,0x0000000c,0x0003003e,0x0000001f,0x0000001d,
        0x0004003d,0x00000007,0x00000023,0x00000022,0x0003003e,0x00000020,0x00000023,0x0004003d,
        0x00000014,0x00000027,0x00000026,0x0003003e,0x00000025,0x00000027,0x000100fd,0x00010038
    };

    static constexpr uint32_t fragmentShader_SPIRV[] =
    {
        0x07230203,0x00010000,0x0008000a,0x00000018,0x00000000,0x00020011,0x00000001,0x0006000b,
        0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
        0x0008000f,0x00000004,0x00000004,0x6e69616d,0x00000000,0x00000009,0x0000000b,0x00000014,
        0x00030010,0x00000004,0x00000007,0x00030003,0x00000002,0x000001a4,0x00040005,0x00000004,
        0x6e69616d,0x00000000,0x00050005,0x00000009,0x756f7370,0x6f635f74,0x0000006c,0x00050005,
        0x0000000b,0x756f7376,0x6f635f74,0x0000006c,0x00040005,0x00000010,0x74786554,0x00657275,
        0x00050005,0x00000014,0x756f7376,0x76755f74,0x00000000,0x00040047,0x00000009,0x0000001e,
        0x00000000,0x00040047,0x0000000b,0x0000001e,0x00000000,0x00040047,0x00000010,0x00000022,
        0x00000000,0x00040047,0x00000010,0x00000021,0x00000000,0x00040047,0x00000014,0x0000001e,
        0x00000001,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,0x00030016,0x00000006,
        0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040020,0x00000008,0x00000003,
        0x00000007,0x0004003b,0x00000008,0x00000009,0x00000003,0x00040020,0x0000000a,0x00000001,
        0x00000007,0x0004003b,0x0000000a,0x0000000b,0x00000001,0x00090019,0x0000000d,0x00000006,
        0x00000001,0x00000000,0x00000000,0x00000000,0x00000001,0x00000000,0x0003001b,0x0000000e,
        0x0000000d,0x00040020,0x0000000f,0x00000000,0x0000000e,0x0004003b,0x0000000f,0x00000010,
        0x00000000,0x00040017,0x00000012,0x00000006,0x00000002,0x00040020,0x00000013,0x00000001,
        0x00000012,0x0004003b,0x00000013,0x00000014,0x00000001,0x00050036,0x00000002,0x00000004,
        0x00000000,0x00000003,0x000200f8,0x00000005,0x0004003d,0x00000007,0x0000000c,0x0000000b,
        0x0004003d,0x0000000e,0x00000011,0x00000010,0x0004003d,0x00000012,0x00000015,0x00000014,
        0x00050057,0x00000007,0x00000016,0x00000011,0x00000015,0x00050085,0x00000007,0x00000017,
        0x0000000c,0x00000016,0x0003003e,0x00000009,0x00000017,0x000100fd,0x00010038
    };

    static const char* vertexShaderHLSL = R"(
        cbuffer Constants
        {
            float4x4 ProjectionMatrix;
        }

        struct VSInput
        {
            float2 pos : ATTRIB0;
            float2 uv  : ATTRIB1;
            float4 col : ATTRIB2;
        };

        struct PSInput
        {
            float4 pos : SV_POSITION;
            float4 col : COLOR;
            float2 uv  : TEXCOORD;
        };

        void main(in VSInput VSIn, out PSInput PSIn)
        {
            PSIn.pos = mul(ProjectionMatrix, float4(VSIn.pos.xy, 0.0, 1.0));
            PSIn.col = VSIn.col;
            PSIn.uv  = VSIn.uv;
        }
    )";

    static const char* pixelShaderHLSL = R"(
        struct PSInput
        {
            float4 pos : SV_POSITION;
            float4 col : COLOR;
            float2 uv  : TEXCOORD;
        };

        Texture2D    Texture;
        SamplerState Texture_sampler;

        float4 main(in PSInput PSIn) : SV_Target
        {
            return PSIn.col * Texture.Sample(Texture_sampler, PSIn.uv);
        }
    )";



    static const char* vertexShaderGLSL = R"(
        #ifdef VULKAN
        #   define BINDING(X) layout(binding=X)
        #   define OUT_LOCATION(X) layout(location=X) // Requires separable programs
        #else
        #   define BINDING(X)
        #   define OUT_LOCATION(X)
        #endif
        BINDING(0) uniform Constants
        {
            mat4 ProjectionMatrix;
        };

        layout(location = 0) in vec2 in_pos;
        layout(location = 1) in vec2 in_uv;
        layout(location = 2) in vec4 in_col;

        OUT_LOCATION(0) out vec4 vsout_col;
        OUT_LOCATION(1) out vec2 vsout_uv;

        #ifndef GL_ES
        out gl_PerVertex
        {
            vec4 gl_Position;
        };
        #endif

        void main()
        {
            gl_Position = ProjectionMatrix * vec4(in_pos.xy, 0.0, 1.0);
            vsout_col = in_col;
            vsout_uv  = in_uv;
        }
    )";

    static const char* pixelShaderGLSL = R"(
        #ifdef VULKAN
        #   define BINDING(X) layout(binding=X)
        #   define IN_LOCATION(X) layout(location=X) // Requires separable programs
        #else
        #   define BINDING(X)
        #   define IN_LOCATION(X)
        #endif
        BINDING(0) uniform sampler2D Texture;

        IN_LOCATION(0) in vec4 vsout_col;
        IN_LOCATION(1) in vec2 vsout_uv;

        layout(location = 0) out vec4 psout_col;

        void main()
        {
            psout_col = vsout_col * texture(Texture, vsout_uv);
        }
    )";

	ImGuiPotatoRenderer::ImGuiPotatoRenderer(IRenderDevice* device,
                                             TextureFormat backBufferFmt,
                                             TextureFormat depthBufferFmt,
                                             unsigned int   initialVertexBufferSize,
                                             unsigned int   initialIndexBufferSize) :
        m_device(device),
        m_backBufferFmt(backBufferFmt),
        m_depthBufferFmt(depthBufferFmt),
        m_vertexBufferSize(initialVertexBufferSize),
        m_indexBufferSize(initialIndexBufferSize)
    {
        // Setup back-end capabilities flags
        IMGUI_CHECKVERSION();
        ImGuiIO& io = ImGui::GetIO();
        io.BackendRendererName = "ImGuiPotatoRenderer";
        io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset; // We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.

        createDeviceObjects();
    }

    ImGuiPotatoRenderer::~ImGuiPotatoRenderer()
    {
    }

    void ImGuiPotatoRenderer::invalidateDeviceObjects()
    {
        delete m_pVB;
        delete m_pIB;
        delete m_pVertexConstantBuffer;
        delete m_pPSO;
        delete m_pFontSRV;
        delete m_pSRB;
    }

    void ImGuiPotatoRenderer::createDeviceObjects()
    {
        invalidateDeviceObjects();

        ShaderCreateInfo shaderCI;
        //shaderCI.UseCombinedTextureSamplers = true;
        //shaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_DEFAULT;

        const auto& deviceCaps = m_device->getDeviceCapabilities();

        // Vertex shader

        shaderCI.desc.type = ShaderType::VERTEX;
        shaderCI.desc.name = "Imgui VS";

        switch (deviceCaps.devType)
        {
            case RenderDeviceType::RENDER_DEVICE_TYPE_VULKAN:
                shaderCI.byteCode = vertexShader_SPIRV;
                shaderCI.byteCodeSize = sizeof(vertexShader_SPIRV);
                break;

            case RenderDeviceType::RENDER_DEVICE_TYPE_D3D11:
            case RenderDeviceType::RENDER_DEVICE_TYPE_D3D12:
                shaderCI.source = vertexShaderHLSL;
                break;

            case RenderDeviceType::RENDER_DEVICE_TYPE_GL:
            case RenderDeviceType::RENDER_DEVICE_TYPE_GLES:
                shaderCI.source = vertexShaderGLSL;
                break;
            default:
                POTATO_FAIL_MSG("Unknown render device type");
        }

        IShader* pVS = m_device->createShader(shaderCI);

        // Fragment shader

        shaderCI.desc.type = ShaderType::FRAGMENT;
        shaderCI.desc.name = "Imgui PS";

        switch (deviceCaps.devType)
        {
            case RenderDeviceType::RENDER_DEVICE_TYPE_VULKAN:
                shaderCI.byteCode = fragmentShader_SPIRV;
                shaderCI.byteCodeSize = sizeof(fragmentShader_SPIRV);
                break;

            case RenderDeviceType::RENDER_DEVICE_TYPE_D3D11:
            case RenderDeviceType::RENDER_DEVICE_TYPE_D3D12:
                shaderCI.source = pixelShaderHLSL;
                break;

            case RenderDeviceType::RENDER_DEVICE_TYPE_GL:
            case RenderDeviceType::RENDER_DEVICE_TYPE_GLES:
                shaderCI.source = pixelShaderGLSL;
                break;

            default:
                POTATO_FAIL_MSG("Unknown render device type");
        }

        IShader* pFS = m_device->createShader(shaderCI);

        GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PSOCreateInfo.PSODesc.name = "ImGUI PSO";
        
        auto& graphicsPipeline = PSOCreateInfo.graphicsPipeline;
        graphicsPipeline.numRenderTargets = 1;
        graphicsPipeline.RTVFormats[0] = m_backBufferFmt;
        graphicsPipeline.DSVFormat = m_depthBufferFmt;
        graphicsPipeline.primitiveTopology = PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        PSOCreateInfo.pVS = pVS;
        PSOCreateInfo.pPS = pFS;

        graphicsPipeline.rasterizerDesc.cullMode = CullMode::CULL_MODE_NONE;
        graphicsPipeline.rasterizerDesc.scissorEnable = true;
        graphicsPipeline.depthStencilDesc.depthEnable = false;

        auto& RT0 = graphicsPipeline.BlendDesc.renderTargets[0];
        RT0.blendEnable = true;
        RT0.srcBlend = BlendFactor::BLEND_FACTOR_SRC_ALPHA;
        RT0.destBlend = BlendFactor::BLEND_FACTOR_INV_SRC_ALPHA;
        RT0.blendOp = BlendOperation::BLEND_OPERATION_ADD;
        RT0.srcBlendAlpha = BlendFactor::BLEND_FACTOR_INV_SRC_ALPHA;
        RT0.destBlendAlpha = BlendFactor::BLEND_FACTOR_ZERO;
        RT0.blendOpAlpha = BlendOperation::BLEND_OPERATION_ADD;
        RT0.renderTargetWriteMask = static_cast<uint8_t>(ColorMask::COLOR_MASK_ALL);

        LayoutElement VSInputs[] //
        {
            {0, 0, 2, ValueType::VT_FLOAT32},    // pos
            {1, 0, 2, ValueType::VT_FLOAT32},    // uv
            {2, 0, 4, ValueType::VT_UINT8, true} // col
        };

        graphicsPipeline.InputLayout.numElements = _countof(VSInputs);
        graphicsPipeline.InputLayout.layoutElements = VSInputs;

        ShaderResourceVariableDesc variables[] =
        {
            { ShaderType::FRAGMENT, "Texture", ShaderResourceVariableType::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC} //
        };
        PSOCreateInfo.PSODesc.resourceLayout.variables = variables;
        PSOCreateInfo.PSODesc.resourceLayout.numVariables = _countof(variables);

        SamplerDesc samLinearWrap;
        samLinearWrap.addressU = TextureAddressMode::TEXTURE_ADDRESS_WRAP;
        samLinearWrap.addressV = TextureAddressMode::TEXTURE_ADDRESS_WRAP;
        samLinearWrap.addressW = TextureAddressMode::TEXTURE_ADDRESS_WRAP;
        ImmutableSamplerDesc imtblSamplers[] =
        {
            { ShaderType::FRAGMENT, "Texture", samLinearWrap} //
        };
        PSOCreateInfo.PSODesc.resourceLayout.immutableSamplers = imtblSamplers;
        PSOCreateInfo.PSODesc.resourceLayout.numImmutableSamplers = _countof(imtblSamplers);

        m_device->createGraphicsPipelineState(PSOCreateInfo, &m_pPSO);

        {
            BufferDesc BuffDesc;
            BuffDesc.uiSizeInBytes = sizeof(float4x4);
            BuffDesc.Usage = USAGE_DYNAMIC;
            BuffDesc.BindFlags = BIND_UNIFORM_BUFFER;
            BuffDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
            m_device->CreateBuffer(BuffDesc, nullptr, &m_pVertexConstantBuffer);
        }

        //delete pVS;
        //delete pFS;

    }

}