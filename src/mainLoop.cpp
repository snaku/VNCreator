#include "mainLoop.h"

namespace mainLoop
{
	void mainLoop(GLFWwindow* window)
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
		}
	}
}