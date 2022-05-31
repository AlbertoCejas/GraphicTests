#include <Core/include/VKSwapChain.h>
#include <Core/include/VKRenderDevice.h>
#include <Core/Types.h>

#include <PotatoEngine/Core/Window.h>

#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan_core.h>

using namespace potato;
using namespace potato::vk;


// TODO: change to a non-glfw-oriented approach

VKSwapChain::VKSwapChain(VKRenderDevice& renderDevice, Window& window)
	: VKSwapChain::Base()
	, m_window(&window)
{
	m_instance = renderDevice.GetVKInstance();

	createSurface();
}

VKSwapChain::~VKSwapChain()
{
}

void VKSwapChain::createSurface()
{
	if (glfwCreateWindowSurface(m_instance, m_window->getNativeWindow(), nullptr, &m_surface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}

	// Check present support

}