#include "Window.h"

// constructeur
Window::Window()
{
	createWindow();
}

// destructeur
Window::~Window()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();
}

// fenêtre
void Window::createWindow()
{
	glfwInit();

	// n'utilise pas OpenGL et ne peut pas changer la taille de la fenêtre
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// création de la fenêtre
	this->window = glfwCreateWindow(LARGEUR, HAUTEUR, "Visual Novel", nullptr, nullptr);
}

// getter
GLFWwindow* Window::getWindow() 
{
	return this->window;
}