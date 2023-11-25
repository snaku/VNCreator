#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Window
{
public:
	Window();
	~Window();

	void createWindow();
	GLFWwindow* getWindow();
private:
	const uint32_t LARGEUR = 800;
	const uint32_t HAUTEUR = 600;

	GLFWwindow* window;
};