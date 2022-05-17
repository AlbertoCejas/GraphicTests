#include <PotatoApp/PotatoApp.h>
#include <iostream>


int main(int argc, char** argv)
{
	PotatoApp app("PotatoApp");

	app.init();

	while (app.shouldExit() == false)
	{
		try
		{
			app.update();
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return EXIT_FAILURE;
		}
	}

	app.shutdown();

	return EXIT_SUCCESS;
}