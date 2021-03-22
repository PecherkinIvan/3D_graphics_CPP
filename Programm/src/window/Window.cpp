#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Window.h"


GLFWwindow* Window::window;

int Window::initialize(int width, int height, const char* title) {
	glfwInit(); //Запуск GLFW
	/*-------------Версия OpenGL------------------*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	/*--------------------------------------------*/
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(width, height, title, nullptr, nullptr); // Создание окна. Аргументы: 1) Ширина окна | 2) Высота окна | 3) Название окна
	if (window == nullptr) {
		std::cerr << "Failed to create GLFW Window" << std::endl;
		glfwTerminate(); // Закрытие окна
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	glViewport(0, 0, width, height);
	return 0;
}


/* Закрытие окна  */
void Window::terminate() {
	glfwTerminate();
}


bool Window::isShouldClose() {
	return glfwWindowShouldClose(window);
}

void Window::setShouldClose(bool flag) {
	glfwSetWindowShouldClose(window, flag);
}

void Window::swapBuffers() {
	glfwSwapBuffers(window);
}
