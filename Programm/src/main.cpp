#include <iostream>
				      
#define GLEW_STATIC  
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*---------GLM---------*/
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace glm;
/*-------------------*/

#include "graphics/Shader.h"
#include "graphics/texture.h"
#include "window/Window.h"
#include "window/Events.h"
#include "window/camera.h"
#include "loaders/png_loading.h"


int WIDTH = 1280; //Ширина экрана
int HEIGHT = 720; //Высота экрана

float vertices[] = { // Массив вершинных данных
	//x    y     z     u     v
   -1.0f,-1.0f, 0.0f, 0.0f, 0.0f,  // NEW
	1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
   -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

	1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
   -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
};

int main() {
	Window::initialize(WIDTH, HEIGHT, "MineCraft C++"); // Запуск системы окна
	Events::initialize(); // Запуск системы ивентов

	glClearColor(0.6f, 0.62f, 0.65f, 1); // Цвет фона RGBA

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat))); // Процедура говорит от куда брать данные для массива атрибутов (Vertex attribute array).
																									     // Аргументы: 1) Индекс вершинного массива | 
																										// 2) Длинна вершинного атрибута (В значении) кол-во координат (3 - трехмерный вектор)
																									   // 3) Тип данных | 4) Нужно ли автоматически нормализововать или они уже нормализованные или это не требуется | 
																									  // 5) Длина шага вершинных данных, длина всех данных на одну вершину в байтах
																									 // 6) НА сколько сдвинут вершинный атрибут относительно начала в байтах
	glEnableVertexAttribArray(0); // Включает указанный вершинный атрибут (vertex attribute array)

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));   ///  NEW
	glEnableVertexAttribArray(1);																			///  NEW

	glBindVertexArray(0); // Отвязывание VAO от точки связывания

	glClearColor(0.6f, 0.62f, 0.65f, 1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Camera* camera = new Camera(vec3(0, 0, 1), radians(90.0f));
	mat4 model(1.0f);
	model = translate(model, vec3(0.5f, 0, 0));

	float lastTime = glfwGetTime();
	float delta = 0.0f;

	float camX = 0.0f;
	float camY = 0.0f;

	float speed = 5;

	/*=============Основной цикл игры====================*/
	while (!Window::isShouldClose()) { 

		float currentTime = glfwGetTime();
		delta = currentTime - lastTime;
		lastTime = currentTime;

		//Events::pullEvents();
		if (Events::jpressed(GLFW_KEY_ESCAPE)) {
			Window::setShouldClose(true); 
		}
		if (Events::jclicked(GLFW_MOUSE_BUTTON_1)) {
			glClearColor(0.8f, 0.4f, 0.2f, 1);
		}

		if (Events::jpressed(GLFW_KEY_TAB)) {
			Events::toogleCursor();
		}

		if (Events::pressed(GLFW_KEY_W)) {
			camera->position += camera->front * delta * speed;
		}
		if (Events::pressed(GLFW_KEY_S)) {
			camera->position -= camera->front * delta * speed;
		}
		if (Events::pressed(GLFW_KEY_D)) {
			camera->position += camera->right * delta * speed;
		}
		if (Events::pressed(GLFW_KEY_A)) {
			camera->position -= camera->right * delta * speed;
		}

		if (Events::_cursor_locked) {
			camY += -Events::deltaY / Window::height * 2;
			camX += -Events::deltaX / Window::height * 2;

			if (camY < -radians(89.0f)) {
				camY = -radians(89.0f);
			}
			if (camY > radians(89.0f)) {
				camY = radians(89.0f);
			}

			camera->rotation = mat4(1.0f);
			camera->rotate(camY, camX, 0);
		}

		glClear(GL_COLOR_BUFFER_BIT);	// Изменение цвета фона

		
		
		/*------Отрисовка вершин-------*/
		shader->use(); // Запуск шейдера
		shader->uniformMatrix("model", model);
		shader->uniformMatrix("projview", camera->getProjection()*camera->getView());
		texture->bind(); // Превязываем текстуру
		glBindVertexArray(VAO); // Запуск VAO
		glDrawArrays(GL_TRIANGLES, 0, 6); // Отрисовка примитивов, параметры: 1) Что отрисововать (Тип примитива) | 2) С какой вершины начать рисовать | 3) Кол-во вершин, которые надо отрисовать
		glBindVertexArray(0); // Отвязывание VAO от точки связывания
		/*----Конец отрисовки вершин--*/

		Window::swapBuffers();
		Events::pullEvents();
	}
	/*=========Конец основного цикла игры==============*/

	delete shader; // Удаление шейдеров и очистка памяти
	delete texture; // Удаление текстур и очистка памяти
	glDeleteBuffers(1, &VBO); // Удаление буфера и очистка памяти
	glDeleteVertexArrays(1, &VAO); // Удаление массива и очистка памяти
	Window::terminate(); // Закрытие окна
	return 0;
}