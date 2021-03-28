#include <iostream>
				      
#define GLEW_STATIC  
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "graphics/Shader.h"
#include "graphics/texture.h"
#include "window/Window.h"
#include "window/Events.h"
#include "loaders/png_loading.h"


int WIDTH = 1280; //Ширина экрана
int HEIGHT = 720; //Высота экрана

float vertices[] = { // Массив вершинных данных
	-1.0f, -1.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
};

int main() {
	Window::initialize(WIDTH, HEIGHT, "MineCraft C++"); // Запуск системы окна
	Events::initialize(); // Запуск системы ивентов

	glClearColor(0.6f, 0.62f, 0.65f, 1); // Цвет фона aRGB

	Shader* shader = load_shader("res/frag.glsl", "res/vert.glsl"); // Загрузка фрагментного и вершинного шейдера
	if(shader == nullptr) {
		std::cerr << "failed to load shader" << std::endl;
		Window::terminate(); //Закрытие окна
		return 1;
	}

	Texture* texture = load_texture("res/img.png");
	if (texture == nullptr) {
		std::cerr << "failed to load texture" << std::endl;
		delete shader;
		Window::terminate(); //Закрытие окна
		return 1;
	}





	/*Создание VAO и VBO
		
		VBO - средство OpenGL, которое позваляет загружать в память GPU(Видеокарты) определенные данные (Координаты вершин, цвета, нормали)

		VAO - "Массив" в котором хранится информация о том, какую часть VBO использовать и как эти данные нужно интерпритировать

	*/ 
	GLuint VAO, VBO; 
	
	glGenVertexArrays(1, &VAO); // Создание объекта вершинных массивов
	glGenBuffers(1, &VBO);	// Создание вершинного буффера

	glBindVertexArray(VAO); // Привязываем VAO «ок, сейчас мы будем что-то делать вот с этим VAO»
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Связываем буффер с точкой связывания «ок, сейчас мы будем что-то делать вот с этим VBO»
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Выделение памяти и загрузка данных в неё. Аргументы: 1) Точка связывания | 2) Размер массива в байтах | 3) Массив верщин | 4) 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat))); // Процедура говорит от куда брать данные для массива атрибутов (Vertex attribute array).
																									     // Аргументы: 1) Индекс вершинного массива | 
																										// 2) Длинна вершинного атрибута (В значении) кол-во координат (3 - трехмерный вектор)
																									   // 3) Тип данных | 4) Нужно ли автоматически нормализововать или они уже нормализованные или это не требуется | 
																									  // 5) Длина шага вершинных данных, длина всех данных на одну вершину в байтах
																									 // 6) НА сколько сдвинут вершинный атрибут относительно начала в байтах
	glEnableVertexAttribArray(0); // Включает указанный вершинный атрибут (vertex attribute array)

	glBindVertexArray(0); // Отвязывание VAO от точки связывания

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	/*=============Основной цикл игры====================*/
	while (!Window::isShouldClose()) { 
		Events::pullEvents();
		if (Events::jpressed(GLFW_KEY_ESCAPE)) {
			Window::setShouldClose(true);
		}
		if (Events::jclicked(GLFW_MOUSE_BUTTON_1)) {
			glClearColor(0.8f, 0.4f, 0.2f, 1);
		}

		glClear(GL_COLOR_BUFFER_BIT);	// Изменение цвета фона

		
		
		/*------Отрисовка вершин-------*/
		shader->use(); // Запуск шейдера
		texture->bind(); // Превязываем текстуру
		glBindVertexArray(VAO); // Запуск VAO
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // Отрисовка примитивов, параметры: 1) Что отрисововать (Тип примитива) | 2) С какой вершины начать рисовать | 3) Кол-во вершин, которые надо отрисовать
		glBindVertexArray(0); // Отвязывание VAO от точки связывания
		/*----Конец отрисовки вершин--*/

		Window::swapBuffers();
		
	}
	/*=========Конец основного цикла игры==============*/

	delete shader; // Удаление шейдеров и очистка памяти
	delete texture; // Удаление текстур и очистка памяти
	glDeleteBuffers(1, &VBO); // Удаление буфера и очистка памяти
	glDeleteVertexArrays(1, &VAO); // Удаление массива и очистка памяти
	Window::terminate(); // Закрытие окна
	return 0;
}