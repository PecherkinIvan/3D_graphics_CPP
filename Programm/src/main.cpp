#include <iostream>
				      // Надо исправить, здесь весь код из первого видео (Где-то есть ошибка, возможно в атребутах или нет поключенной библеотеки)
#define GLEW_STATIC  // Примечание Window, Events это классы 
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "graphics/Shader.h"
#include "window/Window.h"
#include "window/Events.h"

int WIDTH = 1280;
int HEIGHT = 720;

int main() {
	Window::initialize(WIDTH, HEIGHT, "Window 2.0");
	Events::initialize();

	glClearColor(0.6f, 0.62f, 0.65f, 1);

	Shader* shader = load_shader("res/main.glslf", "res/main.glslv");
	if(shader == nullptr) {
		std::cerr << "failed to load shader" << std::endl;
		Window::terminate();
		return 1;
	}

	while (!Window::isShouldClose()) {
		Events::pullEvents();
		if (Events::jpressed(GLFW_KEY_ESCAPE)) {
			Window::setShouldClose(true);
		}
		if (Events::jclicked(GLFW_MOUSE_BUTTON_1)) {
			glClearColor(0.8f, 0.4f, 0.2f, 1);
		}

		glClear(GL_COLOR_BUFFER_BIT);

		Window::swapBuffers();
	}
	Window::terminate();
	return 0;
}