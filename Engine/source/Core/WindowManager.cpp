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

	Window* window = new Window(title, *glfwWindow);
	m_windows.push_back(window);

	return window;
}

void WindowManager::destroyWindow(Window& window)
{
	m_windows.remove(&window);

	delete &window;
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

Window* WindowManager::getPrimaryWindow() const
{
	for (Window* window : m_windows)
	{
		if (window->m_isPrimary)
		{
			return window;
		}
	}
	return nullptr;
}

void WindowManager::setPrimaryWindow(Window& newPrimaryWindow)
{
	for (Window* window : m_windows)
	{
		if (window != &newPrimaryWindow)
		{
			window->m_isPrimary = false;
		}
	}
	newPrimaryWindow.m_isPrimary = true;
}