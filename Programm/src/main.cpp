#include <iostream>
				      
#define GLEW_STATIC  
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "graphics/Shader.h"
#include "window/Window.h"
#include "window/Events.h"

int WIDTH = 1280;
int HEIGHT = 720;

float vertices[] = { // Массив точек для треугольника
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
};

int main() {
	Window::initialize(WIDTH, HEIGHT, "Window 2.0");
	Events::initialize();

	glClearColor(0.6f, 0.62f, 0.65f, 1);

	Shader* shader = load_shader("res/frag.glsl", "res/vert.glsl");
	if(shader == nullptr) {
		std::cerr << "failed to load shader" << std::endl;
		Window::terminate();
		return 1;
	}

	GLuint VAO, VBO; 
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); //Освобождение массива вершин


	while (!Window::isShouldClose()) {
		Events::pullEvents();
		if (Events::jpressed(GLFW_KEY_ESCAPE)) {
			Window::setShouldClose(true);
		}
		if (Events::jclicked(GLFW_MOUSE_BUTTON_1)) {
			glClearColor(0.8f, 0.4f, 0.2f, 1);
		}

		glClear(GL_COLOR_BUFFER_BIT);

		/*Отрисовка треугольника*/
		shader->use(); //Запуск шейдера
		glBindVertexArray(VAO); //Запуск VAO
		glDrawArrays(GL_TRIANGLES, 0, 3); //Запуск отрисовки массива вершин, параметры: 1) Что отрисововать | 2) Индекс элемента | 3) Кол-во вершин, которые надо отобразить0
		glBindVertexArray(0); //Освобождение массива вершин
		/*Конец отрисовки треугольника*/

		Window::swapBuffers();
		
	}
	Window::terminate();
	return 0;
}