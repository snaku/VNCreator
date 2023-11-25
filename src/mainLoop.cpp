#include "mainLoop.h"

namespace VNCreatorApp
{
	void mainLoop(GLFWwindow* window)
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
		}
	}
}