#pragma once

struct GLFWwindow;

namespace potato
{

	class Window
	{
		public:

			~Window();

			const char* getTitle() const;

			int getWidth() const;
			int getHeight() const;

		private:

			friend class WindowManager;

			explicit Window(GLFWwindow& window);

			GLFWwindow* m_window;
	};
	
}