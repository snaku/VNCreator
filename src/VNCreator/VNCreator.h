#pragma once

namespace VNCreator
{
	// TODO: faire la même fonction, mais avec SDL2
#if defined(VNCREATOR_USING_GLFW)
	/*!
	* @brief Initialise Vulkan (instance, surface, physical device, logical device
	* et les extensions), une boucle principale, la swapchain et la pipeline
	*
	* @param window: La fenêtre principale (GLFW)
	*/
	void start(GLFWwindow* window);
#endif // defined(VKCREATOR_INIT_GLFW)
	
#if defined(VNCREATOR_USE_IMAGE)
	/*!
	* @brief Charge une image depuis un chemin
	*
	* @param imagePath: le chemin de l'image
	*/
	void loadImage(const char* imagePath);
#endif // defined(VNCREATOR_USE_IMAGE)
	
}