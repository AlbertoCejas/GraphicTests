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

			Window* getPrimaryWindow() const;
			const std::list<Window*> getWindows() const { return m_windows; }

			void setPrimaryWindow(Window& window);

		private:

			std::list<Window*> m_windows;
	};
}