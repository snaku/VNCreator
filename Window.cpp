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

// fen�tre
void Window::createWindow()
{
	glfwInit();

	// n'utilise pas OpenGL et ne peut pas changer la taille de la fen�tre
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// cr�ation de la fen�tre
	this->window = glfwCreateWindow(LARGEUR, HAUTEUR, "Visual Novel", nullptr, nullptr);
}

// getter
GLFWwindow* Window::getWindow() 
{
	return this->window;
}