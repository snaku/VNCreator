#pragma once
#include <vulkan/vulkan.h>

#include "VulkanInit.h"

#include <string>
#include <stdexcept>
#include <iostream>

class Image
{
public:
	void loadImage(const char* imagePath);
private:
	VulkanInit vulkan;
};