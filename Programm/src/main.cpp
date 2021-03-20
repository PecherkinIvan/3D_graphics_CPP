#include <iostream>
					/// Время 8:17 (https://www.youtube.com/watch?v=Ihn-9EYWOEs&t=216s)
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//#include "window/Window.h"

int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Cube", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed_0" <<std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	/*
	glewExperimental = GL_TRUE;  // При запуске выдаст ошибу
	if (glfwInit() != GLEW_OK)
	{
		std::cout << "Failed_1" << std::endl;
		return -1;
	}*/

	glViewport(0, 0, 1280, 720);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}