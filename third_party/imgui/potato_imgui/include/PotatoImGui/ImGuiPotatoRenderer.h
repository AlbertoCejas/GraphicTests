#pragma once

#include <cstdint>

struct ImDrawData;

namespace potato
{
	struct IRenderDevice;
	struct IDeviceContext;
	struct IBuffer;
	struct IPipelineState;
	struct ITextureView;
	struct IShaderResourceBinding;
	struct IShaderResourceVariable;

	enum class TextureFormat : uint16_t;
	enum class SURFACE_TRANSFORM : uint32_t;

	class ImGuiPotatoRenderer
	{
	public:
		ImGuiPotatoRenderer(
			IRenderDevice* device,
			TextureFormat backBufferFmt,
			TextureFormat depthBufferFmt,
			unsigned int initialVertexBufferSize,
			unsigned int initialIndexBufferSize);
		~ImGuiPotatoRenderer();

		//void newFrame(unsigned int renderSurfaceWidth, unsigned int renderSurfaceHeight, SURFACE_TRANSFORM surfacePreTransform);
		//void endFrame();
		//void renderDrawData(IDeviceContext* pCtx, ImDrawData* pDrawData);
		void invalidateDeviceObjects();
		void createDeviceObjects();
		//void createFontsTexture();
	private:
		//inline float4 TransformClipRect(const ImVec2& DisplaySize, const float4& rect) const;

		IRenderDevice*           m_device = nullptr;
		IBuffer*                 m_pVB = nullptr;
		IBuffer*                 m_pIB = nullptr;
		IBuffer*                 m_pVertexConstantBuffer = nullptr;
		IPipelineState*          m_pPSO = nullptr;
		ITextureView*            m_pFontSRV = nullptr;
		IShaderResourceBinding*  m_pSRB = nullptr;
		IShaderResourceVariable* m_pTextureVar = nullptr;

		const TextureFormat		m_backBufferFmt;
		const TextureFormat		m_depthBufferFmt;
		uint32_t				m_vertexBufferSize = 0;
		uint32_t				m_indexBufferSize = 0;
		uint32_t				m_renderSurfaceWidth = 0;
		uint32_t				m_renderSurfaceHeight = 0;
		//SURFACE_TRANSFORM		m_surfacePreTransform = SURFACE_TRANSFORM_IDENTITY;
	};
}