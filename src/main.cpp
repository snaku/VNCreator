#include "VulkanInit.h"
#include "Window.h"
#include "mainLoop.h"

int main()
{
	try
	{
		Window window;
		VulkanInit vulkan(window);
		VNCreatorApp::mainLoop(window.getWindow());
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}