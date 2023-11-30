#pragma once
#include <GLFW/glfw3.h>

namespace VNCreator
{
	/*
	* @brief Initialise la fenêtre (glfw), Vulkan (instance, surface, physical device, logical device
	* et les extensions), la swapchain et la pipeline
	*/
	void init();

	/*
	* @brief Charge une image depuis un chemin
	* @param imagePath: le chemin de l'image
	*/
	void loadImage(const char* imagePath);
}