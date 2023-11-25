#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <optional>
#include <set>
#include <cstdint>
#include <limits>
#include <algorithm>
#include <array>
#include <fstream>

class Window;

class VulkanInit
{
public:
	VulkanInit(Window& window);
	~VulkanInit();

	void run(Window& window);
	void createInstance();
	void createSurface(Window& window);
	void clean();

private:

#ifdef NDEBUG
	const bool enableVL = false;
#else 
	const bool enableVL = true;
#endif // NDEBUG

	const std::vector<const char*> validationLayers =
	{
		"VK_LAYER_KHRONOS_validation"
	};

	VkInstance instance;
	VkSurfaceKHR surface;
};