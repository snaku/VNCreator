#include "VNCreator.h"
#include "Window.h"
#include "VulkanInit.h"
#include "SwapChain.h"
#include "mainLoop.h"
#include "Image.h"
#include "Pipeline.h"

namespace VNCreator
{
	void init()
	{
		Window window;
		VulkanInit vulkan;
		vulkan.run(window); // swapchain.run
		Pipeline pipeline(vulkan);
		mainLoop::mainLoop(window.getWindow());
		vulkan.clean(); // swapchain.clean
	}

	void loadImage(const char* imagePath)
	{
		Image image;
		image.loadImage(imagePath);
	}
}