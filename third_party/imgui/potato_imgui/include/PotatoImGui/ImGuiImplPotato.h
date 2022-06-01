#pragma once

#include <cstdint>
#include <memory>

struct GLFWwindow;

namespace potato
{
    struct IRenderDevice;
    struct IDeviceContext;
    enum class TextureFormat : uint16_t;
    enum class SURFACE_TRANSFORM : uint32_t;

    class ImGuiPotatoRenderer;

    class ImGuiImplPotato
    {
    public:
        static constexpr const uint32_t DefaultInitialVBSize = 1024;
        static constexpr const uint32_t DefaultInitialIBSize = 2048;

        ImGuiImplPotato(
            GLFWwindow* window,
            IRenderDevice* pDevice, 
            TextureFormat backBufferFmt,
            TextureFormat depthBufferFmt,
            unsigned int initialVertexBufferSize = DefaultInitialVBSize, 
            unsigned int initialIndexBufferSize = DefaultInitialIBSize);
        virtual ~ImGuiImplPotato();

        ImGuiImplPotato(const ImGuiImplPotato&) = delete;
        ImGuiImplPotato(ImGuiImplPotato&&) = delete;
        ImGuiImplPotato& operator = (const ImGuiImplPotato&) = delete;
        ImGuiImplPotato& operator = (ImGuiImplPotato&&) = delete;


        /// Begins new frame

        /// \param [in] RenderSurfaceWidth  - Render surface width not accounting for pre-transform.
        ///                                   Most of the time this is the actual swap chain width.
        /// \param [in] RenderSurfaceHeight - Render surface height not accounting for pre-transform.
        ///                                   Most of the time this is the actual swap chain height.
        /// \param [in] SurfacePreTransform - Render surface pre-transform.
        ///                                   Most of the time this is the swap chain pre-transform.
        // virtual void newFrame(unsigned int renderSurfaceWidth, unsigned int renderSurfaceHeight, SURFACE_TRANSFORM surfacePreTransform);

        // virtual void endFrame();
        // virtual void render(IDeviceContext* pCtx);

        // Use if you want to reset your rendering device without losing ImGui state.
        // void invalidateDeviceObjects();
        // void createDeviceObjects();

        // void updateFontsTexture();

    private:

        std::unique_ptr<ImGuiPotatoRenderer> m_pRenderer;
    };
}