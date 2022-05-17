#include "Core/WindowManager.h"
#include <GLFW/glfw3.h>
#include "Core/Window.h"
#include "Util/Assert.h"

using namespace potato;

WindowManager::~WindowManager()
{
	destroyAll();
}

Window* WindowManager::createWindow(const char* title, int width, int height)
{
	GLFWwindow* glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
	if (glfwWindow == NULL)
	{
		return nullptr;
	}

	Window* window = new Window(*glfwWindow);

	m_windows.push_back(window);
}

void WindowManager::destroyWindow(Window& window)
{
	POTATO_ASSERT_MSG(window.m_window != nullptr, "Attempting to destroy a null window");

	m_windows.remove(&window);

	glfwDestroyWindow(window.m_window);
}

void WindowManager::destroyAll()
{
	for (Window* window : m_windows)
	{
		POTATO_ASSERT_MSG(window->m_window != nullptr, "Attempting to destroy a null window");
		glfwDestroyWindow(window->m_window);
	}

	m_windows.clear();
}