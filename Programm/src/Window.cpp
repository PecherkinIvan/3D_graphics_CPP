#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>>
#include "Window.h"

GLFWwindow* Window::window;

int Window::initialize(int width, int height, const char* title) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed_0" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
}