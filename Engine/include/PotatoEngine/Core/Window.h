#pragma once

struct GLFWwindow;

namespace potato
{
	using NativeWindow = GLFWwindow;

	class Window
	{
		public:

			~Window();

			const char* getTitle() const;

			int getWidth() const;
			int getHeight() const;

			NativeWindow* getNativeWindow() const { return m_window; }

		private:

			friend class WindowManager;

			explicit Window(GLFWwindow& window);

			GLFWwindow* m_window;
	};
	
}