#include <iostream>
				      
#define GLEW_STATIC  
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*---------GLM---------*/
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
/*-------------------*/

#include "graphics/Shader.h"
#include "graphics/texture.h"
#include "graphics/Mesh.h"
#include "graphics/VoxelRenderer.h"
#include "window/Window.h"
#include "window/Events.h"
#include "window/camera.h"
#include "loaders/png_loading.h"
#include "voxels/voxel.h"
#include "voxels/Chunk.h"


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

int attrs[] = {
		3,2, 0
};

int main() {
	Window::initialize(WIDTH, HEIGHT, "MineCraft C++"); // Запуск системы окна
	Events::initialize(); // Запуск системы ивентов
	
	Shader* shader = load_shader("res/frag.glsl", "res/vert.glsl"); // Загрузка фрагментного и вершинного шейдера
	if(shader == nullptr) {
		std::cerr << "failed to load shader" << std::endl;
		Window::terminate(); //Закрытие окна
		return 1;
	}

	Texture* texture = load_texture("res/block.png");
	if (texture == nullptr) {
		std::cerr << "failed to load texture" << std::endl;
		delete shader;
		Window::terminate(); //Закрытие окна
		return 1;
	}

	VoxelRenderer renderer(1024 * 1024 * 8);
	Chunk* chunk = new Chunk();
	Mesh* mesh = renderer.render(chunk);
	glClearColor(0.6f, 0.62f, 0.65f, 1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
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
		mesh->draw(GL_TRIANGLES);
		
		Window::swapBuffers();
		Events::pullEvents();
	}
	/*=========Конец основного цикла игры==============*/

	delete shader; // Удаление шейдеров и очистка памяти
	delete texture; // Удаление текстур и очистка памяти
	delete mesh;
	delete chunk;

	Window::terminate(); // Закрытие окна
	return 0;
}