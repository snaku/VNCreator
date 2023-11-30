#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VulkanInit.h"

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

class Pipeline
{
public:
	Pipeline(VulkanInit& vulkan);

	void createGfxPipeline(VulkanInit& vulkan);

	VkShaderModule createShaderModule(const std::vector<char>& code, VulkanInit& vulkan);
private:
	static std::vector<char> readShader(const std::string& filename);
};