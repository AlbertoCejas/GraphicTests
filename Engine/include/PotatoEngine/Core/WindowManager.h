#pragma once

#include <list>

namespace potato
{
	class Window;

	class WindowManager
	{
		public:

			~WindowManager();

			Window* createWindow(const char* title, int width, int height);
			void destroyWindow(Window& window);

			void destroyAll();

			const std::list<Window*> getWindows() const { return m_windows; }

		private:

			std::list<Window*> m_windows;
	};
}