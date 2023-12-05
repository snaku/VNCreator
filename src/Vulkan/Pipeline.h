#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include "SwapChain.h"

class Swapchain;
class VulkanInit;

class Pipeline
{
public:
	Pipeline(VulkanInit& vulkan, Swapchain& swapchain);

	void createGfxPipeline(VulkanInit& vulkan, Swapchain& swapchain);

	VkShaderModule createShaderModule(const std::vector<char>& code, VulkanInit& vulkan);
private:
	static std::vector<char> readShader(const std::string& filename);
};