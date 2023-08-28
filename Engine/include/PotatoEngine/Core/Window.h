#pragma once

#include <string>

struct GLFWwindow;

namespace potato
{
	using NativeWindow = GLFWwindow;

	class Window
	{
		public:

			struct Size
			{
				int width;
				int height;
			};

			~Window();

			const char* getTitle() const { return m_title.c_str(); }

			int getWidth() const;
			int getHeight() const;
			Size getSize() const;

			NativeWindow* getNativeWindow() const { return m_window; }

		private:

			friend class WindowManager;

			explicit Window(const char* title, GLFWwindow& window, bool isPrimary = false);

			std::string m_title = nullptr;
			GLFWwindow* m_window = nullptr;
			bool m_isPrimary = false;
	};
	
}