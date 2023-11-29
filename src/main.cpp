#include "Window.h"
#include "VulkanInit.h"
#include "SwapChain.h"
#include "mainLoop.h"

int main()
{
	try
	{
		Window window;
		VulkanInit vulkan(window);
		Swapchain swapchain;
		swapchain.run(vulkan);
		VNCreatorApp::mainLoop(window.getWindow());
		swapchain.clean(vulkan);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}