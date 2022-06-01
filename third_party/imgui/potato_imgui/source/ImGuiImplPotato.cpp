#include <PotatoImGui/ImGuiImplPotato.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <PotatoImGui/ImGuiPotatoRenderer.h>
#include <PotatoEngine/Util/Assert.h>

namespace potato
{
    ImGuiImplPotato::ImGuiImplPotato(
        GLFWwindow* window,
        IRenderDevice* pDevice,
        TextureFormat backBufferFmt,
        TextureFormat depthBufferFmt,
        unsigned int initialVertexBufferSize,
        unsigned int initialIndexBufferSize)
    {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = nullptr;
        bool result = ImGui_ImplGlfw_InitForOther(window, true);
        POTATO_FATAL_ASSERT_MSG(result == true, "ImGuiPotato GLFW couldn't be initialized");
        m_pRenderer.reset(new ImGuiPotatoRenderer(pDevice, backBufferFmt, depthBufferFmt, initialVertexBufferSize, initialIndexBufferSize));
    }

    ImGuiImplPotato::~ImGuiImplPotato()
    {
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    //void ImGuiImplPotato::newFrame(unsigned int renderSurfaceWidth, unsigned int renderSurfaceHeight, SURFACE_TRANSFORM surfacePreTransform)
    //{
        //m_pRenderer->NewFrame()
    //}
}