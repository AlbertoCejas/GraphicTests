#include "Core/Window.h"
#include <GLFW/glfw3.h>
#include "Util/Assert.h"

using namespace potato;

Window::~Window()
{
	POTATO_ASSERT_MSG(m_window != nullptr, "Attempting to destroy a null window");
	glfwDestroyWindow(m_window);
	m_window = nullptr;
}

int Window::getWidth() const
{
	int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	return width;
}

int Window::getHeight() const
{
	int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	return height;
}

Window::Size Window::getSize() const
{
	Window::Size size;
	glfwGetWindowSize(m_window, &size.width, &size.height);
	return size;
}

Window::Window(const char* title, GLFWwindow& window, bool isPrimary)
	: m_title(title)
	, m_window(&window)
	, m_isPrimary(isPrimary)
{

}