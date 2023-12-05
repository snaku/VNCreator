#pragma once
#include <vulkan/vulkan.h>
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

#include "Window.h"
#include "SwapChain.h"

class Window;

struct queueFamilyIndices
{
	std::optional<uint32_t> gfxFamily;
	std::optional<uint32_t> presFamily;

	bool isComplete()
	{
		return gfxFamily.has_value() && presFamily.has_value();
	}
};

class VulkanInit
{
public:

	void run(GLFWwindow& window);
	void createInstance();
	void createSurface(GLFWwindow& window);
	void pickPhysicalDevice();
	bool isSuitable(VkPhysicalDevice device);
	void createLogicalDevice();
	bool checkDeviceExt(VkPhysicalDevice device);
	void clean();

	queueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	VkInstance getInstance();
	VkSurfaceKHR getSurface();
	VkPhysicalDevice getPhysicalDevice();
	VkDevice getDevice();

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
	
	const std::vector<const char*> deviceExt =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VkInstance instance;
	VkSurfaceKHR surface;

	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;

	VkQueue gfxQueue;
	VkQueue presQueue;

	Swapchain swapchain;

	float priorities = 1.0;

	std::vector<VkDeviceQueueCreateInfo> processQueueFamilies(const queueFamilyIndices& indices) const;
};