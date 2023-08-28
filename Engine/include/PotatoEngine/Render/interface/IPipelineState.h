#pragma once

#include <cstdint>
#include <PotatoEngine/Render/include/InputLayout.h>
#include <string>

namespace potato
{
	class IShader;

	/// Pipeline state creation attributes
	struct PipelineStateCreateInfo
	{
		/// Pipeline state description
		PipelineStateDesc PSODesc;

		/// Pipeline state creation flags, see Diligent::PSO_CREATE_FLAGS.
		//PSO_CREATE_FLAGS  Flags      DEFAULT_INITIALIZER(PSO_CREATE_FLAG_NONE);
	};

	/// Graphics pipeline state description
	/// This structure describes the graphics pipeline state and is part of the GraphicsPipelineStateCreateInfo structure.
	struct GraphicsPipelineDesc
	{
		/// Blend state description.
		//BlendStateDesc BlendDesc;

		/// 32-bit sample mask that determines which samples get updated 
		/// in all the active render targets. A sample mask is always applied; 
		/// it is independent of whether multisampling is enabled, and does not 
		/// depend on whether an application uses multisample render targets.
		//Uint32 SampleMask DEFAULT_INITIALIZER(0xFFFFFFFF);

		/// Rasterizer state description.
		//RasterizerStateDesc RasterizerDesc;

		/// Depth-stencil state description.
		//DepthStencilStateDesc DepthStencilDesc;

		/// Input layout, ignored in a mesh pipeline.
		InputLayoutDesc inputLayout;
		//D3D12_INDEX_BUFFER_STRIP_CUT_VALUE IBStripCutValue;

		/// Primitive topology type, ignored in a mesh pipeline.
		//PRIMITIVE_TOPOLOGY PrimitiveTopology DEFAULT_INITIALIZER(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

		/// The number of viewports used by this pipeline
		//Uint8 NumViewports           DEFAULT_INITIALIZER(1);

		/// The number of render targets in the RTVFormats array.
		/// Must be 0 when pRenderPass is not null.
		//Uint8 NumRenderTargets       DEFAULT_INITIALIZER(0);

		/// When pRenderPass is not null, the subpass
		/// index within the render pass.
		/// When pRenderPass is null, this member must be 0.
		//Uint8 SubpassIndex           DEFAULT_INITIALIZER(0);

		/// Render target formats.
		/// All formats must be TEX_FORMAT_UNKNOWN when pRenderPass is not null.
		//TEXTURE_FORMAT RTVFormats[8] DEFAULT_INITIALIZER({});

		/// Depth-stencil format.
		/// Must be TEX_FORMAT_UNKNOWN when pRenderPass is not null.
		//TEXTURE_FORMAT DSVFormat     DEFAULT_INITIALIZER(TEX_FORMAT_UNKNOWN);

		/// Multisampling parameters.
		//SampleDesc SmplDesc;

		/// Pointer to the render pass object.
		/// When non-null render pass is specified, NumRenderTargets must be 0,
		/// and all RTV formats as well as DSV format must be TEX_FORMAT_UNKNOWN.
		IRenderPass* renderPass = nullptr;

		/// Node mask.
		//Uint32 NodeMask DEFAULT_INITIALIZER(0);

		//D3D12_CACHED_PIPELINE_STATE CachedPSO;
		//D3D12_PIPELINE_STATE_FLAGS Flags;
	};

	/// Pipeline type
	enum PipelineType : uint8_t
	{
		/// Graphics pipeline, which is used by IDeviceContext::Draw(), IDeviceContext::DrawIndexed(),
		/// IDeviceContext::DrawIndirect(), IDeviceContext::DrawIndexedIndirect().
		PIPELINE_TYPE_GRAPHICS,

		/// Compute pipeline, which is used by IDeviceContext::DispatchCompute(), IDeviceContext::DispatchComputeIndirect().
		PIPELINE_TYPE_COMPUTE,

		/// Mesh pipeline, which is used by IDeviceContext::DrawMesh(), IDeviceContext::DrawMeshIndirect().
		PIPELINE_TYPE_MESH,

		/// Ray tracing pipeline, which is used by IDeviceContext::TraceRays().
		PIPELINE_TYPE_RAY_TRACING,

		PIPELINE_TYPE_LAST = PIPELINE_TYPE_RAY_TRACING
	};

	struct PipelineStateDesc
	{
		std::string name = nullptr;

		/// Pipeline type
		PipelineType pipelineType = PipelineType::PIPELINE_TYPE_GRAPHICS;

		/// Shader resource binding allocation granularity

		/// This member defines allocation granularity for internal resources required by the shader resource
		/// binding object instances.
		//Uint32 SRBAllocationGranularity DEFAULT_INITIALIZER(1);

		/// Defines which command queues this pipeline state can be used with
		//Uint64 CommandQueueMask         DEFAULT_INITIALIZER(1);

		/// Pipeline layout description
		//PipelineResourceLayoutDesc ResourceLayout;

		bool isAnyGraphicsPipeline() const { return pipelineType == PIPELINE_TYPE_GRAPHICS || pipelineType == PIPELINE_TYPE_MESH; }

	};

	/// Graphics pipeline state creation attributes
	struct GraphicsPipelineStateCreateInfo : public PipelineStateCreateInfo
	{
		/// Graphics pipeline state description.
		GraphicsPipelineDesc graphicsPipeline;

		/// Vertex shader to be used with the pipeline.
		IShader* vsPtr = nullptr;

		/// Pixel shader to be used with the pipeline.
		IShader* fsPtr = nullptr;
	};

	// TODO: compute shaders

	class IPipelineState
	{
		public:

			virtual const PipelineStateDesc& getDesc() const = 0;
			//virtual const GraphicsPipelineDesc& getGraphicsPipelineDesc() const = 0;
	};
}