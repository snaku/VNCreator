#include "VNCreator.h"
#include "VulkanInit.h"
#include "Pipeline.h"
#include "SwapChain.h"
#include "mainLoop.h"
#include "Image.h"

namespace VNCreator
{
	namespace
	{
		Image image;
		Swapchain swapchain;
	}

	void start(GLFWwindow* window)
	{
		//Window window;
		VulkanInit vulkan;
		vulkan.run(*window); // swapchain.run
		Pipeline pipeline(vulkan, swapchain);
		mainLoop::mainLoop(window);
		vulkan.clean(); // swapchain.clean
	}

	void loadImage(const char* imagePath)
	{
		image.loadImage(imagePath);
	}
}